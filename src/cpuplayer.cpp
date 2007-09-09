/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE: CpuPlayer implementation 
*
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
*
* Created on 15-lug-2005
* $Id$
*/
#include "stdheader.h"
#include "book.H"
#include <algorithm>

namespace pulchess { namespace logic {

//
// Global: XBOARD post option (default off)
//
bool CPUPlayer::xboardPost = false;

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
      
      if(  pulchess_board->IsInCheck(colour) )
      {
      	pulchess_info("I am in check, gotta do smth!");
      }
      
      // Prova a cercare la mossa nel libro
      BoardValue * bv = new BoardValue(99, Book::bookSize);
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
      
      m->Play();
      pulchess_board->MoveFinalize(m);
      timec->resetTimer();
      evc->Clear();
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
		//
		// Search!
		pulchess_debug("IDAB iteration to depth " << depth);
		IsSearchValid = true;
		value = Alfabeta(depth, depth, colour, BLACK_WINS, WHITE_WINS);
		
		//
		// Show thinking stuff?
		if( CPUPlayer::xboardPost )
		{
			cout << depth     << " "; // ply
			cout << value     << " "; // score
			cout << timec->GetThinkingTime()       << " "; // time
			cout << visitedNodes << " "; // nodes
			cout << (bestMove!=NULL ? bestMove->toString() : "")     << " "; // pv   = mossa
			cout << endl;
		}
		
		//
		// Reset search stats
		visitedNodes = 0;
		
		//
		// Look for quitting condition
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
    list<Piece *> * pList = pulchess_board->ListPieces(turnColour);
    list<Piece *>::iterator pList_iter;
    vector<Move *> mList;
    vector<Move *>::iterator mList_iter;

    Move *currMove = NULL;
    Move *myBest = NULL;
    int val = 0;

	//
	// Stats
	visitedNodes++;

	//
    // Is it a final node?
    //  o  the King is missing
	//  o  we are in the alphabeta leaves
	//  o  the moveResult parameter is helping us with "quiescienza" in order
	//     to prevent to be eated early.
	//
	if( pulchess_board->GetKing(WHITE) == NULL ) return BLACK_WINS * turnColour;
	if( pulchess_board->GetKing(BLACK) == NULL ) return WHITE_WINS * turnColour;
    if( depth <= 0 && moveResult <= PIECE_RANK_BISHOP )
	{
		return pulchess_board->Evaluate(turnColour) * turnColour;
    }
	
    //
    // fase di lettura dalla cache
    //
#ifdef PULCHESS_USEHASHTABLE
    BoardValue *thisBoardVal = NULL, *hashBoardVal = NULL;

    if(depth != startDepth) {
		int retVal;
		thisBoardVal = new BoardValue(depth, evc->getSize());
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
		return pulchess_board->Evaluate(turnColour) * turnColour;
    }
	
    // per tutti i miei pezzi
    // preleva tutte le mosse possibili, ed aggiungile alla lista (mList)
    //
    for(pList_iter = pList->begin(); pList_iter != pList->end(); pList_iter++)
	{
		(*pList_iter)->listMoves( &mList );
    }
    sort(mList.begin(), mList.end());
	
    //
	// per tutte le mosse
	// se la mossa e' buona, migliore delle altre, viene promossa
	// come mossa migliore (bestMove)
	//
	for(mList_iter = mList.begin(); mList_iter != mList.end(); mList_iter++)
    {
		currMove = (*mList_iter);
		
		moveResult = currMove->Play();
		val = -Alfabeta( depth, depth-1, ENEMY(turnColour), -beta, -alfa );
		currMove->Rewind();
		
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
	
	//
	// Se si ritorna alla profondita' iniziale, e quindi stiamo nella chiamata
	// che poi torna all'utente, allora controlliamo se la mossa va salvata
	// (per poi essere applicata).
	//
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

	//
	// Se la ricerca e' valida, allora inserisci la mossa nella hashtable.
	//
#ifdef PULCHESS_USEHASHTABLE
    if( IsSearchValid )
    {
      thisBoardVal = new BoardValue(depth, evc->getSize());
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
    return new Queen(colour);
}

}; // end logic namespace
}; // end pulchess namespace
