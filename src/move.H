/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE: Move implementations
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
 * Created on 24-gen-2005
 * $Id$
 */
#include "stdheader.h"
 
#ifndef _pulchess_logic_move_h_
#define _pulchess_logic_move_h_

// Heuristics for moves
#define PULCHESS_RAT_STD	  0
#define PULCHESS_RAT_PAWNEATS 5
#define PULCHESS_RAT_PAWNPROM 1000
#define MEAN_MOVE_NUMBER      15

// Destroy a collection of Move pointers...
#define moveListDestroy(X) {\
Move * tmpMove = NULL;\
 vector<Move *>::iterator mlit;\
 for(mlit = (X)->begin(); mlit != (X)->end(); mlit++)\
  {tmpMove = (*mlit); delete tmpMove;} }

namespace pulchess { namespace logic {

  class Piece;
  
  class Move
  {

  public:
    //! Move rating
    int rating;
    
	//! Starting place
    coord_t src;

	//! Destination place
    coord_t dst;

	//! The eventually eaten piece
    Piece * deadPiece;

	//! The eventually promoted pawn
    Piece * promotedPawn;
    
    //! The eventually cleared enpassant status (for move's rewind)
    coord_t enpassant;

	//! The old fifty moves counter (for move's rewind)
	unsigned char fiftyMovesRule;
    
    Move();
    Move(coord_t src, coord_t dst, int rating);
    virtual ~Move();

    coord_t GetSrcIdx();
    coord_t GetDstIdx();

    coord_t getX();
    coord_t getY();
    coord_t getSourceX();
    coord_t getSourceY();

	//! Play me on specified board
    virtual int Play();

	//! Rollback me on specified board
    virtual void Rewind();

	//! Copy me
    virtual Move * copy();

	//! Get a string representation of the move
	virtual string toString();
	
	//! Equal operator
	bool operator== (Move &a);
  };

  //! Less operator
  bool  operator< (const Move &a, const Move &b);

   
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

    int Play();
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
    int Play();
    void Rewind();
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
