/*
 * Created on 24-gen-2005
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
 * $Id$
 */
#include "stdheader.h"

#ifndef _PULCHESS_PLAYERIF_H_
#define _PULCHESS_PLAYERIF_H_

namespace pulchess { namespace logic {

  class Board;
  class Move;

/**
 * @author moreno
 */
  class PlayerIF {

  protected:
    Board* _board;
    colour_t _colour;
    Move* _lastMoveReport;      

  public:
    PlayerIF();
    PlayerIF(colour_t colour);
    virtual ~PlayerIF();
    
    colour_t getColour();
    virtual Piece * chooseSoldierPiece() = 0;
    virtual bool isHuman();	
    virtual bool doYourMove() = 0;
    Move*  lastMoveReport();
    void setBoard(Board* b);
    list<Piece *> * getPieceList();
  };
  
};
};
#endif
