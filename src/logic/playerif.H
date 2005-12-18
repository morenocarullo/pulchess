/*
 * Created on 24-gen-2005
 *
 * $Id$
 */
#include "stdheader.h"

#ifndef _PULCHESS_PLAYERIF_H_
#define _PULCHESS_PLAYERIF_H_

namespace pulchess { namespace logic {

  class Board;
  class Move;

/**
 * @author moreno
 */
  class PlayerIF {

  protected:
    Board* _board;
    static bool _isHuman;
    colour_t _colour;
    Move* _lastMoveReport;      

  public:
    PlayerIF();
    PlayerIF(colour_t colour);
    virtual ~PlayerIF();
    
    colour_t getColour();
    virtual Piece * chooseSoldierPiece() = 0;
    virtual bool isHuman();	
    virtual void doYourMove() = 0;
    Move*  lastMoveReport();
    void setBoard(Board* b);
    list<Piece *> * getPieceList();
  };
  
};
};
#endif
