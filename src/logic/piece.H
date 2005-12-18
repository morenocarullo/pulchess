/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Piece interface
 *
 * Created on 15-lug-2005
 * $Id$
 */
#include "logic/stdheader.h"
 
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

  /**
   * @author moreno
   */
  class Piece
  {

  protected:
    byte pos;
    coord_t x;
    coord_t y;
    colour_t colour;
    int moveCount;
    static int allocStat;

  public:
    Piece();
    Piece(colour_t colour);
    ~Piece();
    
    colour_t getColour();
    bool isEnemy(Piece *p);
    coord_t  getX();
    coord_t  getY();
    byte     getPos();
    void moveTo(byte pos);
    int    getMoveCount();
    void   setMoveCount(int ct);
    char   getKindChr();
    char   getColourChr();
    void   rollback(byte pos);

    bool isValidMove_diag(byte pos, Board *b);
    bool isValidMove_croce(byte pos, Board *b);
	bool    hasNextMove(Board* b);
   
    virtual int     getKind() = 0;
    virtual list<Move *> * listMoves(Board* b) = 0;
    virtual bool    isValidMove(byte pos, Board * b) = 0;    
    virtual int     getRank() = 0;
    virtual byte    getValue() = 0;
	virtual byte	getPosEvaluation();
};

};
};

#endif
