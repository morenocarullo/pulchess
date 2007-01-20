/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Queen implementation
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

  Queen::~Queen() {}

  int Queen::GetKind()
  {
    return PIECE_QUEEN;
  }

  int Queen::GetRank()
  {
    return PIECE_RANK_QUEEN; // queen's rank
  }

  coord_t Queen::GetValue()
  {
    return 7 + colour*5;
  }

  // La mossa della regina e' valida se e' in diagonale oppure
  // in orizzontale / verticale.
  //
  bool Queen::IsValidMove(coord_t newpos, Board * b)
  {
    if( IsValidMove_diag(newpos, b) )
      return true;
    
    if( IsValidMove_croce(newpos, b) )
      return true;
    
    return false;
  }

  
  void Queen::listMoves(Board* b, vector<Move *> *mList)
  {

#define queen_add_move(X,Y,GOTOLABEL) { \
   if( COORDSOK((X)+x, (Y)+y) ) { \
     p = b->GetPiece((X)+x,(Y)+y); \
     if( p == NULL ) \
       mList->push_back( new Move( xy2pos((X)+x,(Y)+y), pos, 0) ); \
     else { \
       if( IsEnemy(p) ) \
         mList->push_back ( new Move( xy2pos((X)+x,(Y)+y), pos, 5)); \
       goto GOTOLABEL; \
     } \
   } }\

   Piece * p = NULL;
   int i,j;

    // diagonale verso nord-est
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
	;
  }

};

};
