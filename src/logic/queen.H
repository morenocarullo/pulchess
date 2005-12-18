#include "logic/stdheader.h"

#ifndef _pulchess_logic_queen_h_
#define _pulchess_logic_queen_h

namespace pulchess { namespace logic {

  class Move;
  class Board;

  class Queen : public Piece
  {

  public:
    Queen(colour_t colour) : Piece(colour) {};
    ~Queen();
    
    int getKind();
    int getRank();
    byte getValue();
    bool isValidMove(byte pos, Board * b);
    list<Move *> * listMoves(Board* b);
  };

};
};

#endif
