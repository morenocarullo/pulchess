/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: abstract Player implementation
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

namespace pulchess { namespace logic {

  PlayerIF::PlayerIF(colour_t colour) {
    _colour = colour;
  }

  PlayerIF::~PlayerIF()
  {
  }
  
  colour_t PlayerIF::getColour()
  {
    return _colour;
  }

  void PlayerIF::setBoard(Board* b)
  {
    _board = b;
  }	
  
  //! No, I am not human!
  bool PlayerIF::isHuman()
  {
    return false;
  }
 
  //! Get piece list as a delegated method
  list<Piece *> * PlayerIF::getPieceList()
  {
    return _board->listPieces(getColour());
  }

};
};