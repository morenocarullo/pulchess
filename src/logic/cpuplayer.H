/*
* PROJECT: PulCHESS, a Computer Chess program
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
	
	class Move;
	class HashCache;
	
	//! CpuPlayer is a kind of player
	class CPUPlayer : public PlayerIF
	{
	private:
		//! Pointer to best move
		Move *bestMove;
		
		//! Ply depth
		int plyDeep;
		
		//! Hash table used as transposition table
		HashCache *evc;

		//
		time_t moveCalcTime;
		
		//! TimeControl let us balance move search in a particular time-span
		TimeControl *timec;
		
	public:
		//! Class constructor
		CPUPlayer(colour_t colour, int plyDeep, int moveSeconds);
		
		//! Class destructor
		~CPUPlayer();
		
		//! Request move
		bool doYourMove();
		
		//! Do alphabeta search
		int alfabeta(int depth, colour_t turnColour, int alfa, int beta);
		
		//! Choose a piece in place of a soldier
		Piece * chooseSoldierPiece();
	};
	
};
};

#endif // CPUPLAYER_H
