/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:    CpuPlayer interface
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

#ifndef _pulchess_logic_cpuplayer_h_
#define _pulchess_logic_cpuplayer_h_

#include "player.H"
#include "hashcache.H"

namespace pulchess { namespace logic {
	
	/**! CPUPlayer is a kind of player
	
         It represents the AI player and uses various search techniques
         to find the right move.
	*/
	class CPUPlayer : public Player
	{

	private:
		//! Pointer to best move  (search-specific)
		Move *bestMove;
		
		//! Num of visited nodes  (search-specific)
		unsigned int visitedNodes;
		
		//! Maximum depth reached with quiescence (search-specific)
        int maxPlyReached;
		
		//! Last result of a move
		int moveResult;
		
		//! (max) Ply depth
		int plyDeep;
		
		//! Last search's target ply depth
		int startDepth;
		
		//! Search validity
		bool IsSearchValid;
		
		//! Hash table used as transposition table
		HashCache *evc;

		//! Available time for thinking a move
		time_t moveCalcTime;
		
		/*   TimeControl stuff for controlling thinking time  */
		
    	//! When was the timer started?
		time_t startTime;
	
    	//! Actual computed `death time'
    	time_t deathTime;

    	//! The specified time during a measurement. If it stops, it's 0
    	time_t secToLive;

    	//! Last (real) time took to complete
    	time_t lastRealTime;

    	//! Last desired secToLive specified by startTimer()
    	time_t lastSecToLive;
		
	    //! Start the time specifying a desired duration
	    void TcStartTimer(time_t secToLive);
 
	    //! Stop the timer and get it ready for another measurement
	    void TcResetTimer();

	    //! Eval the remaining time and return true if we have to hurry up
	    bool TcEvalTimeRemaining(unsigned int depth);

		unsigned int TcGetRealTime();
		
		unsigned int TcGetThinkingTime();


	public:
		//! XBOARD OPTION: post
		static bool xboardPost;

		//! Class constructor with almost no params, ok for dummy use
		CPUPlayer(colour_t colour);
	
		//! Class constructor with full params
		CPUPlayer(colour_t colour, int plyDeep, int moveSeconds, bool hashtbl);
		
		//! Class destructor
		~CPUPlayer();
		
		//! Request move
		bool DoMove(string moveCmd);
		
		//! Iterative deepening
		void Idab(int depth);		
		
		//! Do alphabeta search
		int Alfabeta(int depth, int alfa, int beta);
		
		//! Choose a piece in place of a soldier
		Piece * ChoosePawnPiece();
	}; // end class
	
}; // end ns logic
}; // end ns pulchess

#endif // _pulchess_logic_cpuplayer_h_
