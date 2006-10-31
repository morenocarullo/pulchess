/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE:	Pawn interface
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

#ifndef _pulchess_logic_soldier_h_
#define _pulchess_logic_soldier_h

namespace pulchess { namespace logic {

  //! A pawn piece
  class Pawn : public Piece
  {
    
  public:
    //! Create a new pawn, given its colour
    Pawn(colour_t colour) : Piece(colour) {};

	//! What kind of piece am i?
    int GetKind();
	
	//! This piece's rank
    int GetRank();
	
	//! Piece position
    coord_t getValue();
	
	//! Get valid moves list
    list<Move *> * listMoves(Board* b, list<Move *> *mList);
	
	//! Is the specified one a valid move?
    bool isValidMove(coord_t pos, Board * b);
	
	//! Give an evaluation of current position
	int getPosEvaluation();
  };

}; // end logic namespace
}; // end pulchess namespace

#endif
