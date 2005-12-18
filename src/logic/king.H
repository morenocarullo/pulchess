/**
 *
 */

#include "logic/stdheader.h"

#ifndef _pulchess_logic_king_h_
#define _pulchess_logic_king_h

namespace pulchess { namespace logic {

  class King : public Piece
  {
    
  public:
    King(colour_t colour) : Piece(colour) {};

    int getKind();
    int getRank();
    byte getValue();
	byte getPosEvaluation();
    list<Move *> * listMoves(Board* b);
    bool    isValidMove(byte pos, Board * b);
  };

};
};

#endif
