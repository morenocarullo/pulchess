#include "stdheader.h"

namespace pulchess { namespace logic {

  int Tower::getKind()
  {
    return PIECE_TOWER;
  }

  string Tower::getName()
  {
    return "Tower";
  }

  int Tower::getRank()
  {
    return 8;
  }

  byte Tower::getValue()
  {
    return 7 + colour*4;
  }

  bool Tower::isValidMove(byte pos, Board * b)
  {
    return isValidMove_croce(pos, b);
  }

  list<Move *> * Tower::listMoves(Board * b)
  {
    list<Move *> *mList = new list<Move *>();
    Piece      *p     = NULL;

#define tower_add_move(X,Y) \
 if( !COORDSOK((X),(Y)) ) continue; \
 if((X) == x && (Y) == y ) continue; \
 p = b->getPiece((X),(Y)); \
 if(p == NULL) { \
   mList->push_back( new Move( xy2pos((X),(Y)), pos) ); \
   continue; \
 } \
 else if( p != NULL ) { \
    if( isEnemy(p) ) \
	  mList->push_front( new Move( xy2pos((X),(Y)), pos) ); \
     break;  \
 } \

    // proviamo a spostare la torre alla sua destra.
    for(int i=1; i< 8-x ; i++) {
      tower_add_move(x+i, y);
    }
    
    // proviamo a spostare la torre alla sua sinistra
    for(int i=8-x; i>0; i--) {
      tower_add_move(x-i, y); 
    }

    // proviamo a spostare la torre in basso
    for(int i=1; i<8-y; i++) {
      tower_add_move(x, y+i);
    }

    // proviamo a spostare la torre in alto
    for(int i=8-y; i>0; i--) {
      tower_add_move(x, y-i);
    }

    return mList;	
  }
  
};
};
