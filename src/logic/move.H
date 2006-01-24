/*
 * Created on 24-gen-2005
 * $Id$
 */
#include "logic/stdheader.h"
 
#ifndef _pulchess_logic_move_h_
#define _pulchess_logic_move_h_

#define moveListDestroy(X) {\
Move * tmpMove = NULL;\
 list<Move *>::iterator mlit;\
 for(mlit = (X)->begin(); mlit != (X)->end(); mlit++)\
  {tmpMove = (*mlit); delete tmpMove;} }

namespace pulchess { namespace logic {

  class Piece;
  class Board;
  
  /**
   * @author moreno
   */
  class Move
  {

  protected:
    byte    src;
    byte    dst;

    Piece * deadPiece;		
    Piece * promotedSoldier;
    
  public:
    Move();
    Move(byte src, byte dst);
    virtual ~Move();

    byte    getSrcIdx();
    byte    getDstIdx();

    coord_t getX();
    coord_t getY();
    coord_t getSourceX();
    coord_t getSourceY();

    virtual void play(Board *b);
    virtual void rewind(Board *b);
    void commit();
    virtual Move * copy();
    void setDeadPiece(Piece *p);
    Piece * getDeadPiece();
    void setPromotedSoldier(Piece *p);
    Piece * getPromotedSoldier();
    void printAllocInfo();
  };


  // En passant move
  // 
  ////////////////////////////
  class EPMove : public Move {

  private:
    byte eat;
    
  public:
    EPMove(byte dst, byte src, byte eat);

    byte    getEatIdx();
    coord_t getEatX();
    coord_t getEatY();

    void play(Board *b);
    Move * copy();
  };


  // Move-request move
  //
  ////////////////////////////////
  class CoordsMove : public Move {
  public:
    CoordsMove(string move);
  };

#define QUEENSIDE_ROOK true
#define KINGSIDE_ROOK false
  
  class RookMove : public Move {
  private:
    byte kind;
    colour_t pcol;

  public:
    RookMove(bool rookKind, colour_t colour);
    void play(Board *);
    void rewind(Board *);
    Move * copy();
  };

};
};

#endif
