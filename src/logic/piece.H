/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE:	Piece interface
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

#ifndef _pulchess_logic_piece_h_
#define _pulchess_logic_piece_h_

#define PIECE_SOLDIER 0
#define PIECE_BISHOP  5
#define PIECE_KNIGHT  6
#define PIECE_TOWER   7
#define PIECE_QUEEN   8
#define PIECE_KING    9

using namespace std;

namespace pulchess { namespace logic {
	
	class Move;
	class Board;
	
	//!
	class Piece
	{
		
	protected:
		//! Indexed coord
		coord_t pos;
		
		//! X-coord
		coord_t x;
		
		//! Y-coord
		coord_t y;
		
		//! This piece's colour
		colour_t colour;
		
		//! How many times this piece has been moved?
		int moveCount;
		
	public:
		//! Instantiate a new piece (a white one)
		Piece();
		
		//! Instantiate a new piece, with its colour
		Piece(colour_t colour);
		virtual ~Piece();
		
		colour_t getColour();
		bool isEnemy(Piece *p);
		coord_t  getX();
		coord_t  getY();
		coord_t     getPos();
		void moveTo(coord_t pos);
		int    getMoveCount();
		void   setMoveCount(int ct);
		char   getKindChr();
		char   getColourChr();
		
		//! Bring me back to position pos
		void   rollback(coord_t pos);
		
		//! Can I reach "pos" from current position, moving in diagonal?
		bool isValidMove_diag(coord_t pos, Board *b);
		
		//! Can I reach "pos" from current position, moving hor. or vert.?
		bool isValidMove_croce(coord_t pos, Board *b);
		
		//! Is there another move left?
		bool    hasNextMove(Board* b);
		
		//! What kind of piece am i?
		virtual int     getKind() = 0;
		
		//! Get list of valid moves
		virtual list<Move *> * listMoves(Board* b) = 0;
		
		//! Says whether this move is valid or not
		virtual bool    isValidMove(coord_t pos, Board * b) = 0;
		
		//! This piece's rank
		virtual int     getRank() = 0;
		
		//! This piece value for hash function calc
		virtual coord_t    getValue() = 0;
		
		//! Give an evaluation of current position	
		virtual int	getPosEvaluation();
	};
	
}; // end logic namespace
}; // end pulchess namespace

#endif