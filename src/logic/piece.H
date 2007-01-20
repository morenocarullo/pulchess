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

#define PIECE_NONE    -1
#define PIECE_PAWN    1
#define PIECE_BISHOP  2
#define PIECE_KNIGHT  4
#define PIECE_ROOK    8
#define PIECE_QUEEN   16
#define PIECE_KING    32

#define PIECE_RANK_PAWN 16
#define PIECE_RANK_BISHOP 50
#define PIECE_RANK_KNIGHT 50
#define PIECE_RANK_ROOK 75
#define PIECE_RANK_QUEEN 250
#define PIECE_RANK_KING 3000000

using namespace std;

namespace pulchess { namespace logic {
	
	class Move;
	class Board;
	
	//! Represents a generic piece on chessboard
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
		
		//! Get player's colour (white or black)
		colour_t GetColour();

        //! Is the supplied piece p an enemy?
		bool IsEnemy(Piece *p);

        //! Get x position on chessboard
		coord_t  getX();
		
        //! Get y position on chessboard
		coord_t  getY();

        //! Get position (index) on chessboard
		coord_t     getPos();
		
        //! Move this piece to specified position (index)
		void moveTo(coord_t pos);
		int    GetMoveCount();
		void   setMoveCount(int ct);
		char   getKindChr();
		char   getColourChr();
		
		//! Bring me back to position pos
		void   rollback(coord_t pos);
		
		//! Can I reach "pos" from current position, moving in diagonal?
		bool IsValidMove_diag(coord_t pos, Board *b);
		
		//! Can I reach "pos" from current position, moving hor. or vert.?
		bool IsValidMove_croce(coord_t pos, Board *b);
		
		//! Is there another move left?
		bool    hasNextMove(Board* b);
		
		//! What kind of piece am i?
		virtual int     GetKind() = 0;

		//! Get list of valid moves, providing an already allocated list
		virtual void listMoves(Board *b, vector<Move *> *mList) = 0;
		
		//! Says whether this move is valid or not
		virtual bool    IsValidMove(coord_t pos, Board * b) = 0;
		
		//! This piece's rank
		virtual int     GetRank() = 0;
		
		//! This piece value for hash function calc
		virtual coord_t    GetValue() = 0;
		
		//! Give an evaluation of current position	
		virtual int	getPosEvaluation();
	};
	
}; // end logic namespace
}; // end pulchess namespace
#endif