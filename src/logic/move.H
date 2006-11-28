/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE: Move implementations
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
* Created on 24-gen-2005
* $Id$
*/
#include "stdheader.h"
 
#ifndef _pulchess_logic_move_h_
#define _pulchess_logic_move_h_

// Destroy a list of Move pointers...
#define moveListDestroy(X) {\
Move * tmpMove = NULL;\
 list<Move *>::iterator mlit;\
 for(mlit = (X)->begin(); mlit != (X)->end(); mlit++)\
  {tmpMove = (*mlit); delete tmpMove;} }

namespace pulchess { namespace logic {

  class Piece;
  class Board;
  
  class Move
  {

  protected:
	//! Starting place
    coord_t src;

	//! Destination place
    coord_t dst;

	//! The eventually eaten piece
    Piece * deadPiece;

	//! The eventually promoted pawn
    Piece * promotedPawn;
    
  public:
    Move();
    Move(coord_t src, coord_t dst);
    virtual ~Move();

    coord_t    GetSrcIdx();
    coord_t    GetDstIdx();

    coord_t getX();
    coord_t getY();
    coord_t getSourceX();
    coord_t getSourceY();

	//! Play me on specified board
    virtual int Play(Board *b);

	//! Rollback me on specified board
    virtual void Rewind(Board *b);

	//! Copy me
    virtual Move * copy();

	//! Set the eventually eaten piece
    void setDeadPiece(Piece *p);

	//! Get the eventually eaten piece
    Piece * getDeadPiece();

	//! Set the promoted pawn for move rollback
    void setPromotedPawn(Piece *p);

	//! Get back the eventually promoted pawn, or NULL
    Piece * getPromotedPawn();

	//! Get a string representation of the move
	virtual string toString();
	
	//! Equal operator
	bool operator== (Move &a);
	
    //! Less operator
    int  operator< (Move &a);
  };

   
  /**! En passant move
  
   */
  class EPMove : public Move {

  private:
    coord_t eat;
    
  public:
    EPMove(coord_t dst, coord_t src, coord_t eat);

    coord_t getEatIdx();
    coord_t getEatX();
    coord_t getEatY();

    int Play(Board *b);
    Move * copy();
  };


  /**! The "request" move
  
   */
  class CoordsMove : public Move {
  public:
    CoordsMove(string move);
  };

#define QUEENSIDE_ROOK true
#define KINGSIDE_ROOK false
  
  /**! The castling move
  
   */
  class RookMove : public Move {
  private:
    coord_t kind;
    colour_t pcol;

  public:
    RookMove(bool rookKind, colour_t colour);
    int Play(Board *);
    void Rewind(Board *);
    Move * copy();
  };

  /**! An InvalidMoveException is raised when the specified move is not legal

  */
  class InvalidMoveException
  {
    private:
      string msg;
  
    public:
      InvalidMoveException() { msg = "-- nessun messaggio specificato --";  }
      InvalidMoveException(string msg) { this->msg = msg; }
      string getMsg() { return msg; }
  };

};
};
#endif