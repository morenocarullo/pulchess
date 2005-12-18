#include "logic/stdheader.h"

#ifndef _pulchess_logic_soldier_h_
#define _pulchess_logic_soldier_h

namespace pulchess { namespace logic {

  class Soldier : public Piece
  {
    
  public:
    Soldier(colour_t colour) : Piece(colour) {};

    int getKind();
    int getRank();
    byte getValue();
    list<Move *> * listMoves(Board* b);
    bool isValidMove(byte pos, Board * b);
	byte getPosEvaluation();
  };

};
};

#endif
