/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Tower implementation
 *
 **********************************************************************
 * This program is free software; you can redistribute it and/or modify         
 * it under the terms of the GNU General Public License as published by      
 * the Free Software Foundation; either version 2 of the License, or         
 * (at your option) any later version.                                       
 *                                                                           
 * This program is distributed in the hope that it will be useful,           
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          
 * for more details.                                                         
 **********************************************************************
 * 
 * Created on 15-lug-2005
 * $Id$
 */
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

  coord_t Tower::getValue()
  {
    return 7 + colour*4;
  }

  bool Tower::isValidMove(coord_t pos, Board * b)
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
    for(coord_t i=1; i< 8-x ; i++) {
      tower_add_move(x+i, y);
    }
    
    // proviamo a spostare la torre alla sua sinistra
    for(coord_t i=8-x; i>0; i--) {
      tower_add_move(x-i, y); 
    }

    // proviamo a spostare la torre in basso
    for(coord_t i=1; i<8-y; i++) {
      tower_add_move(x, y+i);
    }

    // proviamo a spostare la torre in alto
    for(coord_t i=8-y; i>0; i--) {
      tower_add_move(x, y-i);
    }

    return mList;	
  }
  
};
};
