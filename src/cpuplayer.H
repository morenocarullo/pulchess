/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:    CpuPlayer interface
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

#ifndef _pulchess_logic_cpuplayer_h_
#define _pulchess_logic_cpuplayer_h_

#include "playerif.H"
#include "hashcache.H"
#include "timecontrol.H"

namespace pulchess { namespace logic {
	
	/**! CPUPlayer is a kind of player
	
         It represents the AI player and uses various search techniques
         to find the right move.
	*/
	class CPUPlayer : public Player
	{

	private:
		//! Pointer to best move
		Move *bestMove;
		
		//! Last result of a move
		int moveResult;
		
		//! Ply depth
		int plyDeep;
		
		//! Search validity
		bool IsSearchValid;
		
		//! Hash table used as transposition table
		HashCache *evc;

		//! Available time for thinking a move
		time_t moveCalcTime;
		
		//! TimeControl let us balance move search in a particular time-span
		TimeControl *timec;
		
	public:
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
		int Alfabeta(int startDepth, int depth, colour_t turnColour, int alfa, int beta);
		
		//! Choose a piece in place of a soldier
		Piece * ChoosePawnPiece();
		
	}; // end class
	
}; // end ns logic
}; // end ns pulchess

#endif // _pulchess_logic_cpuplayer_h_
