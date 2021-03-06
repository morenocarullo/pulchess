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
#include <algorithm>
#include <math.h>
#include "stdheader.h"
#include "book.H"

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
		int numOfElements = 64007; // ~4 Mb
		this->evc = new HashCache(numOfElements);
        pulchess_debug("Created hashtable, total maximum size: " << 
                       (numOfElements*sizeof(BoardValue*) + numOfElements*sizeof(BoardValue))/1024 << " Kb." );
	}
	else {
		this->evc = new HashCache(1);
	}
#endif	
	this->moveCalcTime = moveSeconds;
	this->startTime = 0;
    this->lastRealTime = 0;
    this->secToLive = 0;
}

//
// Simple class constructor
//
CPUPlayer::CPUPlayer(colour_t colour) : Player(colour)
{
	this->plyDeep		= 6;
	this->moveCalcTime	= 1;
	this->evc	= new HashCache(1);
	this->startTime = 0;
    this->lastRealTime = 0;
    this->secToLive = 0;
}

//
// Class destructor
//
CPUPlayer::~CPUPlayer()
{
#ifdef PULCHESS_USEHASHTABLE
	delete this->evc;
#endif	
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
	
	  // conventional clock
	  if( _clockincr==0 && _clockmoves>0 && _clockbonus>0 ) {
        TcStartTimer( (time_t)(_clock * 1/_clockmoves ) );		
      }
      // increment clock
      else if( _clockmoves==0 && _clockincr>0 && _clockbonus>0) {
        TcStartTimer( (time_t)(_clockincr + _clock * 0.01) );
      }
      // exact clock
      else if( _clockmoves==0 && _clockincr==0 && _clockbonus>0) {
        TcStartTimer( _clockbonus );
      }
      
      // Prova a cercare la mossa nel libro
      BoardValue * bv = new BoardValue(99, Book::bookSize);
      m = Book::Search( bv );
      delete bv;	
      
      // Se non c'e', usa IDAB
      if( m == NULL ) {
      	this->Idab( plyDeep );
      	m = bestMove;   
      	if( m == NULL ) {
      	  pulchess_debug("No move was found by cpu!");
          return false;
      	}			
      }
      else {
      	pulchess_debug("found move in book!");
      }
      
      m->Play();
      pulchess_board->MoveFinalize(m);
      TcResetTimer();
      pulchess_info("move " << m->toString() << " thought in " << TcGetRealTime() << " seconds");
    }
    catch(InvalidMoveException *e)
    {
  		pulchess_error("Errore nella generazione della mossa:");
  		pulchess_error( e->getMsg() );
  		delete e;
  		exit(1);
    }

#ifdef PULCHESS_USEHASHTABLE
      evc->Clear();
#endif
	
	return true;
}

//
// Iterative deepening Alfa-beta search
//
void
CPUPlayer::Idab(int maxDepth) 
{	
	int depth, value, window=0;
	const int ASP_WINDOW = 10;
    string strBestMove;
	for(depth=2; depth<=maxDepth; depth++) {	
		
		// Search!
        for(window=ASP_WINDOW; window>=0; window -= ASP_WINDOW) {
			pulchess_debug("IDAB iteration to depth " << depth << ", window is " << window);
			
			IsSearchValid = true;
			maxPlyReached = 0;
			startDepth    = depth;
			value         = Alfabeta(depth, BLACK_WINS+window, WHITE_WINS-window);
			strBestMove   = (bestMove!=NULL ? bestMove->toString() : "");
			
			pulchess_debug("  Results: MaxPLY=" << maxPlyReached << ", VisitedNodes=" << visitedNodes << ", BestMove=" << strBestMove);
		
			// Show thinking stuff?
			if( CPUPlayer::xboardPost )
			{
				cout << depth << " "; // ply
				cout << value         << " "; // score
				cout << TcGetThinkingTime()       << " "; // time
				cout << visitedNodes << " "; // nodes
				cout << strBestMove   << " "; // pv   = mossa
				cout << endl;
			}
					
			//
			// Reset search stats
			visitedNodes = 0;
		
			//
			// Look for quitting condition
			if( TcEvalTimeRemaining(depth) ) {
				return;
			}
			
#ifdef PULCHESS_USEHASHTABLE
			pulchess_debug("  Hashcache usage is " << evc->GetOccupation()*100 << "%%");
#endif
        }
	}
}

//
// Alfa-beta pruning search
//
int
CPUPlayer::Alfabeta(int depth, int alfa, int beta) 
{	
  vector<Move *> mList;
  vector<Move *>::iterator mList_iter;
  BoardValue *thisBoardVal = NULL, *hashBoardVal = NULL;
  Move *currMove = NULL, *myBest = NULL;
  int realDepth  = startDepth - depth;
  int val = 0;

	//
	// Stats
	visitedNodes++;
	maxPlyReached = MAX(maxPlyReached,realDepth);

  //
  // Is it a final node?
  //  o  the King is missing
  //  o  we are in the alphabeta leaves
  //  o  the moveResult parameter is helping us with "quiescienza" in order
  //     to prevent to be eated early.
  //
  if( (depth <= 0 && moveResult <= PIECE_RANK_BISHOP) ||
      (pulchess_board->GetKing(WHITE) == NULL) ||
      (pulchess_board->GetKing(BLACK) == NULL) )
  {
    return pulchess_board->Evaluate();
  }
	
#ifdef PULCHESS_USEHASHTABLE
  if(depth != startDepth)
  {
    int retVal;
    thisBoardVal = new BoardValue(depth, evc->getSize());
    hashBoardVal = evc->Get( thisBoardVal->GetHashKey() );
    if( hashBoardVal != NULL && hashBoardVal->UsableFor( thisBoardVal ) )
    {
      retVal = evc->GetValue( thisBoardVal->GetHashKey() );               
      delete thisBoardVal;
      return retVal;
    }
  }
#endif
	
  // Se il tempo stringe,  risaliamo l'albero.
  //
  if( TcEvalTimeRemaining(depth) && (depth != startDepth) )
  {
    IsSearchValid = false;
    return pulchess_board->Evaluate();
  }

  // Generate moves
  pulchess_board->GenerateMoves(mList, false);

  //
	// per tutte le mosse
	// se la mossa e' buona, migliore delle altre, viene promossa
	// come mossa migliore (bestMove)
	//
	for(mList_iter = mList.begin(); mList_iter != mList.end(); mList_iter++)
  {
		currMove   = (*mList_iter);
		moveResult = currMove->Play();
		val        = -Alfabeta( depth-1, -beta, -alfa );
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
  if( depth == startDepth ) {
    if( myBest == NULL ) {
        return 0;
      }
    if( IsSearchValid ) {
        bestMove = myBest->copy();
      }
      else {
        pulchess_debug("No valid move selected due to timeout.");
      }
  }

	//
	// Se la ricerca e' valida, allora inserisci la mossa nella hashtable.
	//
#ifdef PULCHESS_USEHASHTABLE
    if( depth != startDepth && IsSearchValid ) {
      evc->Insert(thisBoardVal, alfa);
    }
    else {
      if( thisBoardVal != NULL ) {
        delete thisBoardVal;
      }
    }
#endif

  moveListDestroy(&mList);
	
	//
	// Add "move's result distance" penalty to handle King mate cases
	//
  return alfa + (startDepth - depth);
}

//
// Always give a new Queen for pawn promotions
//
Piece *
CPUPlayer::ChoosePawnPiece()
{
    return new Queen(colour);
}

//
// Start the internal timer used for thinking.
//
void CPUPlayer::TcStartTimer(time_t secToLive)
{
  pulchess_debug("started timer with " << secToLive << " s.t.l.");
	
  this->startTime = time(NULL);
  this->deathTime = time(NULL) + secToLive;
  this->secToLive = secToLive;
}

//
// Reset the internal timer used for thinking
//
void CPUPlayer::TcResetTimer()
{
  lastRealTime = time(NULL) - startTime;
  lastSecToLive = secToLive;

  deathTime = 0;
  secToLive = 0;
}

//
// Get the total thinking time
//
inline unsigned int CPUPlayer::TcGetThinkingTime()
{
	return time(NULL) - startTime;
}

//
// Do I have to stop thinking?
//
inline bool CPUPlayer::TcEvalTimeRemaining(unsigned int depth)
{
  return ( (deathTime - time(NULL)) < 1.1 );
}

//
// Last total thinking time
//
inline unsigned int CPUPlayer::TcGetRealTime()
{
  return lastRealTime;
}

}; // end logic namespace
}; // end pulchess namespace