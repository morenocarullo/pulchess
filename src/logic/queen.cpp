/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Queen implementation
 *
 * Created on 15-lug-2005
 * $Id$
 */
#include "logic/stdheader.h"

namespace pulchess { namespace logic {

  Queen::~Queen()
  {
  }

  int Queen::getKind()
  {
    return PIECE_QUEEN;
  }

  int Queen::getRank()
  {
    return 9; // queen's rank
  }

  byte Queen::getValue()
  {
    return 7 + colour*5;
  }

  // La mossa della regina e' valida se e' in diagonale oppure
  // in orizzontale / verticale.
  //
  bool Queen::isValidMove(byte newpos, Board * b)
  {
    if( isValidMove_diag(newpos, b) )
      return true;
    
    if( isValidMove_croce(newpos, b) )
      return true;
    
    return false;
  }

  
  list<Move *> * Queen::listMoves(Board* b)
  {

#define queen_add_move(X,Y,GOTOLABEL) { \
   if( COORDSOK((X)+x, (Y)+y) ) { \
     p = b->getPiece((X)+x,(Y)+y); \
     if( p == NULL ) \
       mList->push_back( new Move( xy2pos((X)+x,(Y)+y), pos) ); \
     else { \
       if( isEnemy(p) ) \
         mList->push_front ( new Move( xy2pos((X)+x,(Y)+y), pos)); \
       goto GOTOLABEL; \
     } \
   } }\

   list<Move *> * mList = new list<Move *>();
   Piece * p = NULL;
   int i,j;

  loop_1: // diagonale verso nord-est
    for(i=1; i<8; i++) {
      queen_add_move(i,i,loop_2);
    }

  loop_2: // diagonale verso sud-ovest
    for(i=-1; i>-8; i--) {
      queen_add_move(i,i,loop_3);
    }

   loop_3: // diagonale verso nord-ovest
    for(i=1,j=-1; i<8 && j>-8; i++, j--) {
      queen_add_move(i,j,loop_4);
    }

   loop_4: // diagonale verso sud-est
    for(i=1,j=-1; i<8 && j>-8; i++, j--) {
      queen_add_move(j,i,loop_5);
    }

   loop_5: // verso l'alto
    for(i=1; i<8; i++) {
      queen_add_move(0,i,loop_6);
    }

   loop_6: // verso il basso
    for(i=-1; i>-8; i--) {
      queen_add_move(0,i,loop_7);
    }

   loop_7: // verso dx
    for(int i=1; i<8; i++) {
      queen_add_move(i,0,loop_8);
    }

   loop_8: // verso sx
    for(i=-1; i>-8; i--) {
      queen_add_move(i,0,end);
    }

  end:
    return mList;
  }

};

};
