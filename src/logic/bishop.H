/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Bishop interface
 *
 * Created on 22-lug-2005
 * $Id$
 */
#include "logic/stdheader.h"

#ifndef _pulchess_logic_bishop_h_
#define _pulchess_logic_bishop_h_

namespace pulchess { namespace logic {

  class Bishop : public Piece
  {

  public:
    Bishop(colour_t colour) : Piece(colour) {};

    int getKind();
    int getRank();
    byte getValue();
    bool isValidMove(byte pos, Board * b);
    list<Move *> * listMoves(Board* b);
  };

};
};

#endif
