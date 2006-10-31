/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Board and BoardValue interfaces
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

#ifndef _pulchess_logic_board_h_
#define _pulchess_logic_board_h_

namespace pulchess { namespace logic {

  // class pre definition
  class PlayerIF;

  /**! This class represents the board and game status

  */
  class Board
  {  
  private:
    
    //! List of white pieces
    list<Piece *> whiteList;

    //! List of black pieces
    list<Piece *> blackList;

    //! List of game moves.
    list<Move *>  gameMoveList;

    //! Map of the pieces
    Piece * _map[64];

    //! Pointer to the white King      
    King * _whiteKing;

    //! Pointer to the black King
    King * _blackKing;

    //! Pointer to the white Player
    PlayerIF * _whitePlayer;

    //! Pointer to the black Player
    PlayerIF * _blackPlayer;
    
    //! Put the piece in the board, for first time insertion
    void _putPiece(const coord_t x, const coord_t y, Piece* p);

  public:
    //! Class ctor
    Board(PlayerIF * white, PlayerIF * black);

    //! Class d'stor
    ~Board();

    //! Actual turn
    colour_t turn;

    //! Move count (total moves playied)
    int moveCount;
    
    //! Get the piece at position x,y
    Piece * GetPiece(const coord_t x, const coord_t y);

    //! Get the piece at position index pos
    Piece * GetPiece(const coord_t pos);
 
    //! Set piece p at position index pos
    void SetPiece(const coord_t pos, Piece *p);

    //! Add piece p in list
    void PieceListAdd(Piece *p);

    //! Delete piece p from list
    void PieceListDel(Piece *p);

    //! List pieces for player "colour"
    list<Piece *> * ListPieces(const colour_t colour);

    //! Get the specified player
    PlayerIF* GetPlayer(colour_t colour);

    //! Get the specified king
    Piece* GetKing(const colour_t colour);

    //! Is game finished?
    bool IsGameFinished();

    //! Who is winning?
    int WhoWins();

    //! Get total move count
    int GetMoveCount();

    //! Returns the move list
    list<Move *> * GetMoveList();

    //! Rolls back last move
    bool MoveRollback();

    //! Confirms a move and save it in move list
    void MoveFinalize(Move *move);

    //! Returns last played move
    Move * GetLastMove();

    //! Evaluate board status
    int Evaluate(colour_t colour);
    
    //! Is the specified king in check?
    int IsInCheck(const colour_t colour);
	bool CanEatThis(coord_t pos, const colour_t colour);
	int  CanEatThis(coord_t pos, const colour_t colour, bool countTimes);
    bool CanDefendCheck(const colour_t colour);
    Move * CheckDefenseMove(const colour_t colour);
  };

  /**! Represents the board in a compact way for caches and stuff like that

   */
  class BoardValue {
  private:
    coord_t map[64];
    coord_t depth;
    int hashkey;

  public:
    BoardValue(Board *b, coord_t depth, unsigned int dstTableSize);
	BoardValue(const coord_t * rec, unsigned int dstTableSize);
    ~BoardValue();
	bool operator== (BoardValue &a);
	bool operator!= (BoardValue &a);

    unsigned int GetHashKey();
    coord_t GetDepth();
    bool UsableFor(BoardValue *b);
    coord_t* GetMap();
  };

}; // end ns logic
}; // end ns pulchess

#endif