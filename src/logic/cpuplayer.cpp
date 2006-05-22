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
CPUPlayer::CPUPlayer(colour_t colour, int plyDeep, int moveSeconds, bool hashtbl) : PlayerIF(colour)
{
	this->plyDeep = plyDeep;
// TODO: permettere di creare cpuplayer senza hashcache per l'autothinking	
#ifdef PULCHESS_USEHASHTABLE
	if( hashtbl ) {
		this->evc = new HashCache(12317);
	}
	else {
		this->evc = new HashCache(1);
	}
#endif	
	this->timec = new TimeControl();
	this->moveCalcTime = moveSeconds;
}

// simple class constructor
CPUPlayer::CPUPlayer(colour_t colour) : PlayerIF(colour)
{
	//CPUPlayer(colour,6,1,false);	
	this->plyDeep		= 6;
	this->moveCalcTime	= 1;
	this->evc	= new HashCache(1);
	this->timec = new TimeControl();
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
		
		if(  _board->isInCheck(getColour()) ) {
			pulchess_log("[info] I am in check, gotta do smth!");
		//	m = _board->checkDefenseMove(getColour());
		//	goto execmove;
		}
		
		// Prova a cercare la mossa nel libro
		BoardValue * bv = new BoardValue(_board, 99, Book::bookSize);
		m = Book::search( bv );
		delete bv;	
		
		// Se non c'e', usa alphabeta
		if( m == NULL ) {
			pulchess_log("[info] move not found in book.");
			
			_board->switchAutoThinking(getColour());
			this->alfabeta( plyDeep, getColour(), BLACK_WINS, WHITE_WINS );
			_board->switchAutoThinking(getColour());
		
			m = bestMove;   
			if( m == NULL ) {
				pulchess_log("[warn] no move was found!");
			}			
		}
		else {
			pulchess_log("[info] trovata mossa da libro!");
		}
//execmove:		
		m->play(_board);
		m->commit();
		timec->resetTimer();
		printf("[info] move thought in %d seconds\n", (int)timec->getRealTime());
    }
    catch(InvalidMoveException *e) {
		pulchess_log("Errore nella generazione della mossa:");
		pulchess_log( e->getMsg() );
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
    Move * idMyBest = NULL, *currMove = NULL;
    bool stopIterative = false;
    static bool ffprob = true;
	
#ifdef PULCHESS_USEHASHTABLE	
    BoardValue *thisBoardVal = NULL, *hashBoardVal = NULL;
#endif

    // e' un nodo finale? (ovvero: manca un re?)
    //
    if( _board->getKing(WHITE) == NULL ) return BLACK_WINS;
    if( _board->getKing(BLACK) == NULL ) return WHITE_WINS;
	
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
	
    // Se il tempo stringe,  risaliamo l'albero.
    //
    if( timec->evalTimeRemaining(depth) && (plyDeep != depth) ) {
		return 0;
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
    //for(int d=1; d<=depth, depth==plyDeep; d++) {
    int d = (depth == plyDeep) ? 1 : depth;
    while(!stopIterative) {
		Move *myBest = NULL;

#ifdef DEBUG
      if( depth == plyDeep ) {
	printf("[info] IDAB iteration to depth %d\n", d);
      }
#endif
		
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
		
		// se sono alla radice, non ho raggiunto la profondita' massima
		// ed il tempo non e' scaduto allora prosegui con l'IDAB search.
		// prima pero' salva la miglior mossa trovata.
		//
		// TODO: we have to provide a way to assure that a move
		// is always generated even if in lo-time mode		
		if( depth == plyDeep && d<depth && !timec->evalTimeRemaining(depth) ) {
			d++;
			idMyBest = myBest;
			pulchess_log("[info] move saved");
			continue;
		}

		stopIterative = true;
	}
	
	if( depth == plyDeep )
	{
		if( idMyBest == NULL ) return 0;
		bestMove = idMyBest->copy();
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
