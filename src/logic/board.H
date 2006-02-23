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

#define pos2x(P)    ( P%8 )
#define pos2y(P)    ( P/8 )
#define xy2pos(X,Y) ( (Y) * 8 + (X) )

namespace pulchess { namespace logic {

  class Piece;
  class PlayerIF;

  // Rappresenta la scacchiera
  //
  class Board {
    
  private:
    
    /* properties */
    list<Piece *> whiteList;
    list<Piece *> blackList;
    Piece * _map[64];          
    King * _whiteKing;
    King * _blackKing;
    PlayerIF * _whitePlayer;
    PlayerIF * _blackPlayer;
    PlayerIF * _backupPlayer;
    
    /* methods */
    void _checkCoords(const coord_t x, const coord_t y);      
    void _putPiece(const coord_t x, const coord_t y, Piece* p);

  public:

    Board(PlayerIF * white, PlayerIF * black);
    ~Board();

    // TODO: usare dei metodi di accesso!
    colour_t turn;
    int moveCount;
    
    Piece * getPiece(const coord_t x, const coord_t y);
    Piece * getPiece(const coord_t idx);
    void    setPiece(const coord_t pos, Piece *p);
    void    pieceListAdd(Piece *p);
    void    pieceListDel(Piece *p);

    list<Piece *> * listPieces(const colour_t colour);
    PlayerIF * getPlayer(colour_t colour);
    Piece * getKing(const colour_t colour);
    bool isGameFinished();
    int getMoveCount();
    int evaluate();
    int whoWins();
    int isInCheck(const colour_t colour);
	bool canEatThis(coord_t pos, const colour_t colour);
	int  canEatThis(coord_t pos, const colour_t colour, bool countTimes);
    bool canDefendCheck(const colour_t colour);
    Move * checkDefenseMove(const colour_t colour);
    void switchAutoThinking(const colour_t colour);
  };

  // Rappresenta la scacchiera in modo compatto
  /////////////////////////////////////////////
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

    unsigned int getHashKey();
    coord_t getDepth();
    bool usableFor(BoardValue *b);
    coord_t* getMap();		
	void print();
  };

};
};

#endif
