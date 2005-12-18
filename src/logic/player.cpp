/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: abstract Player implementation
 *
 * Created on 15-lug-2005
 * $Id$
 */
#include "logic/stdheader.h"

namespace pulchess { namespace logic {

  bool PlayerIF::_isHuman = false;

  PlayerIF::PlayerIF(colour_t colour) {
    _colour = colour;
  }

  PlayerIF::~PlayerIF()
  {

  }
  
  colour_t PlayerIF::getColour() {
    return this->_colour;
  }	
  
  bool PlayerIF::isHuman() {
    return _isHuman;
  }
  
  void PlayerIF::setBoard(Board* b) {
    _board = b;
  }
  
  Move* PlayerIF::lastMoveReport() {
    return _lastMoveReport;
  }
  
  list<Piece *> * PlayerIF::getPieceList()
  {
    _board->listPieces(getColour());
  }

};
};

