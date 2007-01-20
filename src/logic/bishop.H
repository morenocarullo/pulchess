/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Bishop interface
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
 * Created on 22-lug-2005
 * $Id$
 */
#include "stdheader.h"

#ifndef _pulchess_logic_bishop_h_
#define _pulchess_logic_bishop_h_

namespace pulchess { namespace logic {

  //! A bishop (B) piece	
  class Bishop : public Piece
  {
    public:
      Bishop(colour_t colour) : Piece(colour) {};
	
      int     GetKind();
      int     GetRank();
      coord_t GetValue();
      bool    IsValidMove(coord_t pos, Board * b);

      void listMoves(Board* b, vector<Move *> *mList);
  };

}; // end logic namespace
}; // end pulchess namespace

#endif