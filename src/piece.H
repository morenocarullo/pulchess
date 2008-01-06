/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:	Piece interface
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

#define PIECE_RANK_PAWN   16
#define PIECE_RANK_BISHOP 50
#define PIECE_RANK_KNIGHT 50
#define PIECE_RANK_ROOK   80
#define PIECE_RANK_QUEEN  250
#define PIECE_RANK_KING   1000000

using namespace std;

namespace pulchess { namespace logic {
	
	class Move;
	
	//! Represents a generic piece on chessboard
	class Piece
	{
	protected:
				
		//! Can I reach "pos" from current position, moving in diagonal?
		bool   IsValidMove_diag(coord_t pos);
		
		//! Can I reach "pos" from current position, moving hor. or vert.?
		bool   IsValidMove_croce(coord_t pos);
		
	public:
  
		//! Indexed coord (here for speed, should be protected)
		coord_t pos;
		
		//! X-coord (here for speed, should be protected)
		coord_t x;
		
		//! Y-coord (here for speed, should be protected)
		coord_t y;
  
		//! How many times this piece has been moved?
		//short int moveCount;
		
		//! This piece's colour
		colour_t colour;
	
		//! Instantiate a new piece (a white one)
		Piece();
		
		//! Instantiate a new piece, with its colour
		Piece(colour_t colour);
		virtual ~Piece();
		
        //! Is the supplied piece p an enemy?
		bool IsEnemy(Piece *p);

        //! Get x position on chessboard
		coord_t  getX();
		
        //! Get y position on chessboard
		coord_t  getY();

        //! Get position (index) on chessboard
		coord_t  getPos();
		
        //! Move this piece to specified position (index)
		void   moveTo(coord_t pos);
		char   getKindChr();
		char   getColourChr();
		
		string ToString();
		
		//! Bring me back to position pos
		void   Rollback(coord_t pos);
		
		//! What kind of piece am i?
		virtual int  GetKind() = 0;

		//! Get list of valid moves, providing an already allocated list
		virtual void listMoves(vector<Move *> *mList) = 0;
		
		//! Says whether this move is valid or not
		virtual bool IsValidMove(coord_t pos) = 0;
		
		//! This piece's rank
		virtual int  GetRank() = 0;
		
		//! This piece value for hash function calc
		virtual coord_t GetValue() = 0;
		
		//! Give an evaluation of current position	
		virtual int	getPosEvaluation();
	};


  //! A pawn piece
  class Pawn : public Piece
  {
    
  public:
    //! Create a new pawn, given its colour
    Pawn(colour_t colour) : Piece(colour) {};

    int     GetKind();
    int     GetRank();
    coord_t GetValue();
    bool    IsValidMove(coord_t pos);
    int     getPosEvaluation();

    void listMoves(vector<Move *> *mList);
  };


  //! The Knight (N) piece.
  class Knight : public Piece
  {
  
  public:
    Knight(colour_t colour) : Piece(colour) {};

    int     GetKind();
    int     GetRank();
    coord_t GetValue();
    bool    IsValidMove(coord_t pos);
	
    void listMoves(vector<Move *> *mList);
  };
  
  
  //! A bishop (B) piece	
  class Bishop : public Piece
  {
    public:
      Bishop(colour_t colour) : Piece(colour) {};
	
      int     GetKind();
      int     GetRank();
      coord_t GetValue();
      bool    IsValidMove(coord_t pos);

      void listMoves(vector<Move *> *mList);
  };  


  //! The Rook (R) piece
  class Rook : public Piece
  {
    public:
      Rook(colour_t colour) : Piece(colour) {};

      int     GetRank();
      int     GetKind();
      coord_t GetValue();
      bool    IsValidMove(coord_t pos);
      int     getPosEvaluation();

     void listMoves(vector<Move *> *mList);  
  };  


  //! The Queen (Q) piece
  class Queen : public Piece
  {

  public:
    Queen(colour_t colour) : Piece(colour) {};
    ~Queen();
    
    int     GetKind();
    int     GetRank();
    coord_t GetValue();
    bool    IsValidMove(coord_t pos);

    void listMoves(vector<Move *> *mList);
  };


  //! The King (K) piece
  class King : public Piece
  {
    
  public:
    King(colour_t colour) : Piece(colour) {};

    int     GetKind();
    int     GetRank();
    coord_t GetValue();
    int     getPosEvaluation();
    void    listMoves(vector<Move *> *mList);
    bool    IsValidMove(coord_t pos);
  };	
	
}; // end logic namespace
}; // end pulchess namespace
#endif
