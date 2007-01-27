/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:	   Knight implementation
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

static coord_t posvaltbl[2][64] = 
{
{
	0,8,0,0,0,0,8,0,
	0,0,0,8,8,0,0,0,
	0,0,0,8,8,0,0,0,
	0,0,8,8,8,8,0,0,
	0,0,8,8,8,8,0,0,
	0,0,0,8,8,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
},	
{
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,8,8,0,0,0,
	0,0,8,8,8,8,0,0,
	0,0,8,8,8,8,0,0,
	0,0,0,8,8,0,0,0,
	0,0,0,8,8,0,0,0,
	0,8,0,0,0,0,8,0
}
};

namespace pulchess { namespace logic {

  int Knight::GetKind()
  {
    return PIECE_KNIGHT; 
  }

  int Knight::GetRank()
  {
    return PIECE_RANK_KNIGHT;
  }

  coord_t Knight::GetValue()
  {
    return (coord_t)7 + colour*2;
  }
  
int Knight::getPosEvaluation()
{
	return posvaltbl[ colour == WHITE ? 0 : 1 ][int(pos)];
}

  bool Knight::IsValidMove(coord_t newpos, Board* b) 
  {
    bool result = false;
    Piece* p;
    coord_t newx  = pos2x(newpos), newy = pos2y(newpos);

    // se le nuove coordinate sono non valide... niente da fare.
    if( !OKCOORDS(newpos) )
      return false;
    
    /**/ if( newx == x+1 && newy == y+2 )
      result = true;

    else if( newx == x+1 && newy == y-2 )
      result = true;

    else if( newx == x-1 && newy == y+2 ) 
      result = true;

    else if( newx == x-1 && newy == y-2 )
      result = true;

    else if( newx == x+2 && newy == y+1 )
      result = true;

    else if( newx == x-2 && newy == y+1 )
      result = true;

    else if( newx == x+2 && newy == y-1)
      result = true;

    else if( newx == x-2 && newy == y-1)
      result = true;

    // se la posizione e' valida, allora controlla che sia possibile
    // spostarci la pedina (casella vuota oppure occupata da nemico)
    if(result) {
      p = b->GetPiece(newpos);
      return ( p == NULL || IsEnemy(p) );
    }

    return false;
  }


  void Knight::listMoves(Board* b, vector<Move *> *mList)
  {
    Piece      *p     = NULL;

#define add_knight_move(X,Y) \
 if( COORDSOK(X,Y) ) { \
   p = b->GetPiece((X),(Y)); \
   if( p == NULL ) \
    mList->push_back( new Move(xy2pos((X),(Y)), pos, 0) ); \
   else if ( IsEnemy(p) )\
    mList->push_back( new Move(xy2pos((X),(Y)), pos, 10) );\
 }\

    add_knight_move(x+1, y+2);
    add_knight_move(x+1, y-2);
    add_knight_move(x-1, y+2);
    add_knight_move(x-1, y-2);
    add_knight_move(x+2, y+1);
    add_knight_move(x+2, y-1);
    add_knight_move(x-2, y+1);
    add_knight_move(x-2, y-1);
  }

};
};
