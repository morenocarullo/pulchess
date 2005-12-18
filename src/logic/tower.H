#include "logic/stdheader.h"

#ifndef _PULCHESS_LOGIC_TOWER_H_
#define _PULCHESS_LOGIC_TOWER_H_

namespace pulchess { namespace logic {

class Piece;

class Tower : public Piece {
 public:
  Tower(colour_t colour) : Piece(colour) {};

  string getName();
  int getRank();
  int getKind();
  byte getValue();
  bool isValidMove(byte pos, Board * b);
  list<Move *> * listMoves(Board * b);  
};
 
};
};

#endif //_TOWER_H_
