/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE: CpuPlayer implementation 
**********************************************************************
* This program is free software; you can redistribute it and/or modify         
* it under the terms of the GNU General Public License as published by      
* the Free Software Foundation; either version 2 of the License, or         
* (at your option) any later version.                                       
*                                                                           
* This program is distributed in the hope that it will be useful,           
* but WITHOUT ANY WARRANTY; without even the implied warranty of            
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          
* for more details.                                                         
**********************************************************************
* Created on 15-lug-2005
* $Id$
*/
#include "stdheader.h"
#include "book.H"

#define ACCEPTMOVE() ( ffprob = ffprob ? false : true )

namespace pulchess { namespace logic {
	
// class constructor
CPUPlayer::CPUPlayer(colour_t colour, int plyDeep, int moveSeconds) : PlayerIF(colour)
{
	this->plyDeep = plyDeep;
// TODO: permettere di creare cpuplayer senza hashcache per l'autothinking	
#ifdef PULCHESS_USEHASHTABLE
	this->evc = new HashCache(1102317);
#endif	
	this->timec = new TimeControl();
	this->moveCalcTime = moveSeconds;
}

// class destructor
CPUPlayer::~CPUPlayer()
{
#ifdef PULCHESS_USEHASHTABLE
	delete this->evc;
#endif	
	delete this->timec;
}

// play a move!
bool CPUPlayer::doYourMove() /* throws ... */
{
    Move * m = NULL;
    bestMove = NULL;
	
    try {
      
		timec->startTimer(moveCalcTime);
		
		// Prova a cercare la mossa nel libro
		m = Book::search( new BoardValue(_board, 99, Book::bookSize) );
		
		// Se non c'e', usa alphabeta
		if( m == NULL ) {
			cerr << "Mossa non trovata nel libro." << endl;
			
			_board->switchAutoThinking(getColour());
			this->alfabeta( plyDeep, getColour(), BLACK_WINS, WHITE_WINS );
			_board->switchAutoThinking(getColour());
		
			m = bestMove;   
			if( m == NULL ) {
				cerr << "Errore : non e' stata trovata alcuna mossa!" << endl;
			}
		}
		else {
			cerr << "Trovata mossa da libro!" << endl;
		}
		
		m->play(_board);
		m->commit();
		timec->resetTimer();
		
		//cerr << "Mossa scelta." << endl;
		//printf("da x:%d,y:%d    a x:%d,y:%d\n", m->getSourceX(), m->getSourceY(), m->getX(), m->getY());
		//printf("time   req:%d real:%d\n", int(timec->getRequestedTime()), int(timec->getRealTime()) );
		//printf("hashtable hit:%d miss:%d\n", evc->getStatsHit(), evc->getStatsMiss());
    }
    catch(InvalidMoveException *e) {
		cerr << "Errore nella generazione della mossa:" << endl;
		cerr << e->getMsg() << endl;
		exit(1);
    }
	
    _lastMoveReport = m;
	
	return true;
}


// Esegue la ricerca della mossa.
//
///////////////////////////////////////////////////////////////////////////
#define PROMOTEGOODMOVE 				{ \
	mList.push_front( currMove ); \
		mList.erase(mList_iter); \
} \

int
CPUPlayer::alfabeta(int depth, colour_t turnColour, int alfa, int beta) 
{	
    list<Piece *> * pList = _board->listPieces(turnColour);
    list<Piece *>::iterator pList_iter;
    list<Move *> mList;
    list<Move *>::iterator mList_iter;
    int val = 0, best = 0, alfab = 0, betab = 0;
    Move * myBest = NULL, *currMove = NULL;
	static bool ffprob = true;
	
#ifdef PULCHESS_USEHASHTABLE	
    BoardValue *thisBoardVal = NULL, *hashBoardVal = NULL;
#endif
	
    //
    // fase di lettura dalla cache
    //
#ifdef PULCHESS_USEHASHTABLE
    if(depth != plyDeep) {
		int retVal;
		thisBoardVal = new BoardValue(_board, depth, evc->getSize());
		hashBoardVal = evc->get( thisBoardVal->getHashKey() );
		if( hashBoardVal != NULL && hashBoardVal->usableFor( thisBoardVal ) ) {
			retVal = evc->getValue( thisBoardVal->getHashKey() );               
			delete thisBoardVal;
			evc->statsHit();
			return retVal;
		}
		delete thisBoardVal;
		evc->statsMiss();
    }
#endif
	
    // Se il tempo stringe, valutiamo la posizione corrente e risaliamo l'albero.
    //
    if( timec->evalTimeRemaining(depth) && (plyDeep != depth) ) {
		return _board->evaluate();
    }
    
    // stiamo valutando la "foglia" dell'albero di gioco.
    // in realta', non e' una foglia vera e propria (ovvero "la fine del gioco"),
    // ma una foglia virtuale, stabilita in base alla capacita' dell'algoritmo di
    // "vedere avanti", 
    if( depth == 0 ) {
		return _board->evaluate();
    }
	
    // per tutti i miei pezzi
    // preleva tutte le mosse possibili, ed aggiungile alla lista (mList)
    //
    for(pList_iter = pList->begin(); pList_iter != pList->end(); pList_iter++) {
		(*pList_iter)->listMoves( _board, &mList );
    }
	
    // ripetendo piu' volte con profondita' crescente e' possibile
    // "passare" con valori di alfa e beta migliori, ottimizzando i tempi
    //
    for(int d=1; d<=depth; d++) {
		
		// per tutte le mosse
		// se la mossa e' buona, migliore delle altre, viene promossa
		// come mossa migliore (bestMove)
		//
		for(mList_iter = mList.begin(); mList_iter != mList.end(); mList_iter++) {
			currMove = (*mList_iter);
			
			currMove->play( _board );
			
			val = alfabeta( d - 1, ENEMY(turnColour), alfa, beta );
			
			currMove->rewind( _board );
			
			//
			// Decidiamo se la mossa deve essere promossa a "mossa migliore"
			//
			if( (turnColour == WHITE && val >= best) ||
				(turnColour == BLACK && val <= best ))  {
				
				best = val;
				
				//
				// se il valore e' diverso da quello attuale,
				// oppure non e' stata ancora scelta la mossa
				//
				if( val != best || myBest == NULL) {
					myBest = currMove;
				}
				
				//
				// altrimenti, la mossa viene scelta in modo probabilistico,
				// dato che dalla funzione di valutazione questa e' identica
				// alla vecchia "mossa migliore".
				//
				else if( ACCEPTMOVE() ) { 
					myBest = currMove;					
				}
			}
			
			
			// if( piu_a_suo_vantaggio() )
			// stiamo valutando il risultato del nemico. Se ci viene un valore piu' a suo vantaggio,
			// dobbiamo scartare questo ramo!      
			//
			alfab = alfa;
			betab = beta;
			if( val > alfa ) { alfa = val; }
			if( val < beta ) { beta = val; }
			if( (turnColour == WHITE && val < betab) ||
				(turnColour == BLACK && val > alfab ) ) {
				//
				// la mossa ha permesso di "tagliare" l'albero. Va salvata ed utilizzata come
				// prima mossa da testare alla prossima iterazione della iterative deepening.
				//
				PROMOTEGOODMOVE;
				break;
			}            
		}
		
		
    }
	
	if( depth == plyDeep )
	{
		bestMove = myBest->copy();
	}
#ifdef PULCHESS_USEHASHTABLE
    else
	{
		thisBoardVal = new BoardValue(_board, depth, evc->getSize());
		evc->insert(thisBoardVal, best);
	}
#endif
	
	moveListDestroy(&mList);
	
    return best;
}


// Richiede un pezzo da posizionare al posto di un pedone promosso.
//
///////////////////////////////////////////////////////////////////
Piece *
CPUPlayer::chooseSoldierPiece()
{
    return new Queen(getColour());
}

};
};
