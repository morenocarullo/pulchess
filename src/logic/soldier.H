/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Soldier interface
 *
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
 * 
 * Created on 15-lug-2005
 * $Id$
 */
#include "logic/stdheader.h"

#ifndef _pulchess_logic_soldier_h_
#define _pulchess_logic_soldier_h

namespace pulchess { namespace logic {

  class Soldier : public Piece
  {
    
  public:
    Soldier(colour_t colour) : Piece(colour) {};

    int getKind();
    int getRank();
    byte getValue();
    list<Move *> * listMoves(Board* b);
    bool isValidMove(byte pos, Board * b);
	byte getPosEvaluation();
  };

};
};

#endif
