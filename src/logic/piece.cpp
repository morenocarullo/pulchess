#include "stdheader.h"

namespace pulchess { namespace logic {

  int Piece::allocStat = 0;

  Piece::Piece()
  {
    moveCount = -1;
    x = -1;
    y = -1;
    pos = -1;
  }

  Piece::Piece(colour_t colour)
  {
    moveCount = -1;
    x = -1;
    y = -1;
    pos = -1;  
    this->colour = colour;
  }

  Piece::~Piece()
  {
  }
  
  colour_t Piece::getColour()
  {
    return colour;
  }

  //
  // NULL check MUST be done before calling this!
  //
  bool Piece::isEnemy(Piece *p)
  {
    return (getColour() != p->getColour());
  }

  char Piece::getColourChr()
  {
    return ( colour == WHITE ? 'w' : 'b' );
  }

  char Piece::getKindChr()
  {
    int kinda = getKind();

    switch(kinda) {

    case PIECE_QUEEN:
      return 'q';
      break;

    case PIECE_KING:
      return 'k';
      break;

    case PIECE_KNIGHT:
      return 'n';
      break;

    case PIECE_TOWER:
      return 'r';
      break;

    case PIECE_SOLDIER:
      return 'p';
      break;

    case PIECE_BISHOP:
      return 'b';
      break;

    default:
      return '-';
      break;
    }
  }

  coord_t Piece::getX()
  {
    return x;
  }

  coord_t Piece::getY()
  {
    return y;
  }

  byte Piece::getPos()
  {
    return pos;
  }
  
  byte Piece::getPosEvaluation()
  {
	return 0;
  }

  void Piece::moveTo(byte newpos)
  {
    this->x   = pos2x(newpos);
    this->y   = pos2y(newpos);
    this->pos = newpos;
    moveCount++;
  }

  void Piece::rollback(byte newpos)
  {
    this->x   = pos2x(newpos);
    this->y   = pos2y(newpos);
    this->pos = newpos;
    moveCount--;
  }

  int Piece::getMoveCount()
  {
    return moveCount;
  }

  void Piece::setMoveCount(int ct)
  {
    moveCount = ct;
  }

  bool Piece::isValidMove_diag(byte newpos, Board *b)
  {
    int
      int_x    = x,
      int_y    = y,
      int_newx = pos2x(newpos),
      int_newy = pos2y(newpos);

    Piece *p;

    if( !OKCOORDS(newpos)  ) return false;
    if( pos == newpos )      return false;

    if( abs(int_y - int_newy) == abs(int_x - int_newx) ) {
      int incrX, incrY;
      int i,j;
      incrX = ( int_x > int_newx ? -1 : 1 );
      incrY = ( int_y > int_newy ? -1 : 1 );
      for( i=int_x+incrX, j=int_y+incrY;
           i!= int_newx && j!= int_newy;
           i+=incrX, j+=incrY) {
        p =  b->getPiece(i,j);
        if( p != NULL )
          return false;
      }
      p = b->getPiece(newpos);
      if( p == NULL || ( p != NULL && isEnemy(p) )) {
        return true;
      }
    }

    return false;
  }

  bool Piece::isValidMove_croce(byte newpos, Board *b)
  {
    int
      int_x    = x,
      int_y    = y,
      int_newx = pos2x(newpos),
      int_newy = pos2y(newpos);

    Piece *p;

    if( !OKCOORDS(newpos)  ) return false;
    if( pos == newpos )      return false;

    // ci stiamo muovendo in verticale?
    //
    if( abs(int_x - int_newx) == 0 ) {
      int incr = ( int_y > int_newy ? -1 : 1 );
      for( int i = int_y+incr; i != int_newy; i += incr ) {
        p = b->getPiece(int_x, i);
        if( p != NULL )
          return false;
      }
      p =  b->getPiece(int_newx, int_newy);
      if( p == NULL || ( p != NULL && isEnemy(p) ) ) {
        return true;
      }
    }


    // ci stiamo muovendo in orizzontale?
    //
    else if( abs(int_y - int_newy) == 0 ) {
      int incr = ( int_x > int_newx ? -1 : 1 );
      for( int i = int_x+incr; i != int_newx; i += incr ) {
        p = b->getPiece(i, int_y);
        if( p != NULL )
          return false;
      }
      p =  b->getPiece(int_newx, int_newy);
      if( p == NULL || ( p != NULL && isEnemy(p) ) ) {
        return true;
      }
    }

    return false;
  }


  // C'e' almeno una mossa possibile ?
  //
  bool Piece::hasNextMove(Board* b)
  {
    list<Move *> * mList = listMoves(b);
    list<Move *>::iterator mListIter;
    bool val = !mList->empty();
	moveListDestroy(mList);
    delete mList;
    return val;
  }
};
};
