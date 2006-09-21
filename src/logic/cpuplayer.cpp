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
      
      if(  _board->isInCheck(getColour()) )
      {
      	pulchess_info("I am in check, gotta do smth!");
      }
      
      // Prova a cercare la mossa nel libro
      BoardValue * bv = new BoardValue(_board, 99, Book::bookSize);
      m = Book::search( bv );
      delete bv;	
      
      // Se non c'e', usa alphabeta
      if( m == NULL )
      {
      	pulchess_debug("move not found in book.");
      	
      	_board->switchAutoThinking(getColour());
      	this->idab( plyDeep );
      	_board->switchAutoThinking(getColour());
      
      	m = bestMove;   
      	if( m == NULL ) {
      	  pulchess_debug("no move was found in book!");
      	}			
      }
      else
      {
      	pulchess_debug("found move in book!");
      }

      if( m == NULL )
      {
         return false;    
      }
      
      //
      // TODO: bisogna avere la certezza che la mossa non metta
      //       il re in scacco!
      //
      
      m->play(_board);
      m->commit();
      timec->resetTimer();
      
      printf("[info] move %s thought in %d seconds\n", m->toString().c_str(), (int)timec->getRealTime());
    }
    catch(InvalidMoveException *e)
    {
  		pulchess_error("Errore nella generazione della mossa:");
  		pulchess_error( e->getMsg() );
  		delete e;
  		exit(1);
    }
	
    _lastMoveReport = m;
	
	return true;
}

//
// Iterative deepening Alfa-beta search
//
void
CPUPlayer::idab(int maxDepth) 
{	
	int depth, value;
	for(depth=2; depth<=maxDepth; depth++)
	{	
		pulchess_debug("IDAB iteration to depth " << depth);
		value = alfabeta(depth, depth, getColour(), BLACK_WINS, WHITE_WINS);
		if( timec->evalTimeRemaining(depth) )
		{
			return;
		}
	}
}

//
// Alfa-beta pruning search
//
int
CPUPlayer::alfabeta(int startDepth, int depth, colour_t turnColour, int alfa, int beta) 
{	
    list<Piece *> * pList = _board->listPieces(turnColour);
    list<Piece *>::iterator pList_iter;
    list<Move *> mList;
    list<Move *>::iterator mList_iter;

    Move *currMove = NULL;
    Move *myBest = NULL;
    int val = 0;
	
#ifdef PULCHESS_USEHASHTABLE	
    BoardValue *thisBoardVal = NULL, *hashBoardVal = NULL;
#endif

    // E' un nodo finale? 
    //  o  manca il re
	//  o  siamo nelle foglie dell'albero alfabeta
	if( _board->getKing(WHITE) == NULL ) return BLACK_WINS * turnColour;
	if( _board->getKing(BLACK) == NULL ) return WHITE_WINS * turnColour;
    if( depth == 0 )
	{
		return _board->evaluate(turnColour) * turnColour;
    }
	
    //
    // fase di lettura dalla cache
    //
#ifdef PULCHESS_USEHASHTABLE
    if(depth != startDepth) {
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
    if( timec->evalTimeRemaining(depth) && (depth != startDepth) )
	{
		return _board->evaluate(turnColour) * turnColour;
    }
	
    // per tutti i miei pezzi
    // preleva tutte le mosse possibili, ed aggiungile alla lista (mList)
    //
    for(pList_iter = pList->begin(); pList_iter != pList->end(); pList_iter++)
	{
		(*pList_iter)->listMoves( _board, &mList );
    }

	if( bestMove != NULL && depth == startDepth )
	{
		mList.push_front(bestMove);
	}
	

	// per tutte le mosse
	// se la mossa e' buona, migliore delle altre, viene promossa
	// come mossa migliore (bestMove)
	//
	for(mList_iter = mList.begin(); mList_iter != mList.end(); mList_iter++)
    {
		currMove = (*mList_iter);
		
		currMove->play( _board );
		val = -alfabeta( depth, depth-1, ENEMY(turnColour), -beta, -alfa );
		currMove->rewind( _board );
		
		if( val >= beta ) {
			alfa = beta;
			break;
		}
		if( val > alfa ) {
			myBest = currMove;
			alfa = val;
		}
		else if( val == alfa && myBest == NULL ) {
			myBest = currMove;	
		} 
	}
	
    if( depth == startDepth )
    {
     	if( myBest == NULL ) return 0;
//		printf("valore mossa migliore: %d\n", alfa);
    	bestMove = myBest->copy();
    }

#ifdef PULCHESS_USEHASHTABLE
    thisBoardVal = new BoardValue(_board, depth, evc->getSize());
    evc->insert(thisBoardVal, alfa);
#endif
	
    moveListDestroy(&mList);
	
    return alfa;
}

// Richiede un pezzo da posizionare al posto di un pedone promosso.
//
///////////////////////////////////////////////////////////////////
Piece *
CPUPlayer::choosePawnPiece()
{
    return new Queen(getColour());
}

};
};
