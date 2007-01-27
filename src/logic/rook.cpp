/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Rook implementation
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
 * Created on 15-lug-2005
 * $Id$
 */
#include "stdheader.h"

namespace pulchess { namespace logic {

  int Rook::GetKind()
  {
    return PIECE_ROOK;
  }

  int Rook::GetRank()
  {
    return PIECE_RANK_ROOK;
  }

  coord_t Rook::GetValue()
  {
    return 7 + colour*4;
  }

  bool Rook::IsValidMove(coord_t pos, Board * b)
  {
    return IsValidMove_croce(pos, b);
  }

  void Rook::listMoves(Board * b, vector<Move *> *mList)
  {
    Piece      *p     = NULL;

#define tower_add_move(X,Y) \
 if( !COORDSOK((X),(Y)) ) continue; \
 if((X) == x && (Y) == y ) continue; \
 p = b->GetPiece((X),(Y)); \
 if(p == NULL) { \
   mList->push_back( new Move( xy2pos((X),(Y)), pos, 0) ); \
   continue; \
 } \
 else if( p != NULL ) { \
    if( IsEnemy(p) ) \
	  mList->push_back( new Move( xy2pos((X),(Y)), pos, 15) ); \
     break;  \
 } \

    // proviamo a spostare la torre alla sua destra.
    for(coord_t i=x+1; i<8 ; i++) {
      tower_add_move(i, y);
    }
    
    // proviamo a spostare la torre alla sua sinistra
    for(coord_t i=x-1; i>=0; i--) {
      tower_add_move(i, y); 
    }

    // proviamo a spostare la torre in basso
    for(coord_t i=y-1; i>=0; i--) {
      tower_add_move(x, i);
    }

    // proviamo a spostare la torre in alto
    for(coord_t i=y+1; i<8; i++) {
      tower_add_move(x, i);
    }
  }
  
};
};