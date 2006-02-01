/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Knight interface
 *
 * Created on 15-lug-2005
 * $Id$
 */
#include "stdheader.h"

#ifndef _pulchess_logic_knight_h_
#define _pulchess_logic_knight_h

namespace pulchess { namespace logic {

  class Knight : public Piece
  {
  
  public:
    Knight(colour_t colour) : Piece(colour) {};
    int getKind();
    int getRank();
    coord_t getValue();
    list<Move *> * listMoves(Board* b);
    bool isValidMove(coord_t pos, Board * b);
	int getPosEvaluation();
  };

};
};

#endif
