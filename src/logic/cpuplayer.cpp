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
#include <algorithm>

namespace pulchess { namespace logic {
	
//
// Class constructor
// TODO: param moveSeconds no more used
//
CPUPlayer::CPUPlayer(colour_t colour, int plyDeep, int moveSeconds, bool hashtbl) : Player(colour)
{
	this->plyDeep = plyDeep;
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

//
// Simple class constructor
//
CPUPlayer::CPUPlayer(colour_t colour) : Player(colour)
{
	this->plyDeep		= 6;
	this->moveCalcTime	= 1;
	this->evc	= new HashCache(1);
	this->timec = new TimeControl();
}

//
// Class destructor
//
CPUPlayer::~CPUPlayer()
{
#ifdef PULCHESS_USEHASHTABLE
	delete this->evc;
#endif	
	delete this->timec;
}

//
// Play a move!
//
bool CPUPlayer::DoMove(string moveCmd) /* throws ... */
{
   Move * m = NULL;
   bestMove = NULL;

   moveResult = 0;
      
   try
   {
      pulchess_debug("Remaining seconds in timecontrol: " << _clock << " Moves so far: " << _moves);
	
      timec->startTimer( _clock * (_moves+1) / 100 );
      
      if(  _board->IsInCheck(GetColour()) )
      {
      	pulchess_info("I am in check, gotta do smth!");
      }
      
      // Prova a cercare la mossa nel libro
      BoardValue * bv = new BoardValue(_board, 99, Book::bookSize);
      m = Book::Search( bv );
      delete bv;	
      
      // Se non c'e', usa alphabeta
      if( m == NULL )
      {
      	pulchess_debug("move not found in book.");
      	this->Idab( plyDeep );
      	m = bestMove;   
      	if( m == NULL )
        {
      	  pulchess_debug("No move was found by cpu!");
          return false;
      	}			
      }
      else
      {
      	pulchess_debug("found move in book!");
      }
      
      m->Play(_board);
      _board->MoveFinalize(m);
      timec->resetTimer();
      pulchess_info("move " << m->toString() << " thought in " << timec->getRealTime() << " seconds");
    }
    catch(InvalidMoveException *e)
    {
  		pulchess_error("Errore nella generazione della mossa:");
  		pulchess_error( e->getMsg() );
  		delete e;
  		exit(1);
    }
	
	return true;
}

//
// Iterative deepening Alfa-beta search
//
void
CPUPlayer::Idab(int maxDepth) 
{	
	int depth, value;
	for(depth=2; depth<=maxDepth; depth++)
	{	
		pulchess_debug("IDAB iteration to depth " << depth);
		IsSearchValid = true;
		value = Alfabeta(depth, depth, GetColour(), BLACK_WINS, WHITE_WINS);
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
CPUPlayer::Alfabeta(int startDepth, int depth, colour_t turnColour, int alfa, int beta) 
{	
    list<Piece *> * pList = _board->ListPieces(turnColour);
    list<Piece *>::iterator pList_iter;
    vector<Move *> mList;
    vector<Move *>::iterator mList_iter;

    Move *currMove = NULL;
    Move *myBest = NULL;
    int val = 0;

	//
    // Is it a final node?
    //  o  the King is missing
	//  o  we are in the alphabeta leaves
	//  o  the moveResult parameter is helping us with "quiescienza" in order
	//     to prevent to be eated early.
	//
	if( _board->GetKing(WHITE) == NULL ) return BLACK_WINS * turnColour;
	if( _board->GetKing(BLACK) == NULL ) return WHITE_WINS * turnColour;
    if( depth <= 0 && moveResult <= PIECE_RANK_BISHOP )
	{
		return _board->Evaluate(turnColour) * turnColour;
    }
	
    //
    // fase di lettura dalla cache
    //
#ifdef PULCHESS_USEHASHTABLE
    BoardValue *thisBoardVal = NULL, *hashBoardVal = NULL;

    if(depth != startDepth) {
		int retVal;
		thisBoardVal = new BoardValue(_board, depth, evc->getSize());
		hashBoardVal = evc->Get( thisBoardVal->GetHashKey() );
		if( hashBoardVal != NULL && hashBoardVal->UsableFor( thisBoardVal ) ) {
			retVal = evc->GetValue( thisBoardVal->GetHashKey() );               
			delete thisBoardVal;
			return retVal;
		}
		delete thisBoardVal;
    }
#endif
	
    // Se il tempo stringe,  risaliamo l'albero.
    //
    if( timec->evalTimeRemaining(depth) && (depth != startDepth) )
	{
		IsSearchValid = false;
		return _board->Evaluate(turnColour) * turnColour;
    }
	
    // per tutti i miei pezzi
    // preleva tutte le mosse possibili, ed aggiungile alla lista (mList)
    //
    for(pList_iter = pList->begin(); pList_iter != pList->end(); pList_iter++)
	{
		(*pList_iter)->listMoves( _board, &mList );
    }
    sort(mList.begin(), mList.end());

	/*if( bestMove != NULL && depth == startDepth )
	{
		mList.push_back(bestMove);
	}*/
	
    //
	// per tutte le mosse
	// se la mossa e' buona, migliore delle altre, viene promossa
	// come mossa migliore (bestMove)
	//
	for(mList_iter = mList.begin(); mList_iter != mList.end(); mList_iter++)
    {
		currMove = (*mList_iter);
		
		moveResult = currMove->Play( _board );
		val = -Alfabeta( depth, depth-1, ENEMY(turnColour), -beta, -alfa );
		currMove->Rewind( _board );
		
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
    	if( IsSearchValid )
        {
          bestMove = myBest->copy();
        }
        else
        {
          pulchess_debug("No valid move selected due to timeout.");
        }
    }

#ifdef PULCHESS_USEHASHTABLE
    if( IsSearchValid )
    {
      thisBoardVal = new BoardValue(_board, depth, evc->getSize());
      evc->Insert(thisBoardVal, alfa);
    }
#endif

    moveListDestroy(&mList);
	
    return alfa;
}


//
// Always give a new Queen for pawn promotions
//
Piece *
CPUPlayer::ChoosePawnPiece()
{
    return new Queen(GetColour());
}

}; // end logic namespace
}; // end pulchess namespace