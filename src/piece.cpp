/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:	Piece implementation
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
#include <sstream>

namespace pulchess { namespace logic {

  Piece::Piece()
  {
    //moveCount = -1;
    x = -1;
    y = -1;
    pos = -1;
	this->colour = WHITE;
  }

  Piece::Piece(colour_t colour)
  {
    //moveCount = -1;
    x = -1;
    y = -1;
    pos = -1;  
    this->colour = colour;
  }

  Piece::~Piece()
  {
  }
  
  //
  // NULL check MUST be done before calling this!
  //
  bool Piece::IsEnemy(Piece *p)
  {
    return (colour != p->colour);
  }

  char Piece::getColourChr()
  {
    return ( colour == WHITE ? 'w' : 'b' );
  }

  char Piece::getKindChr()
  {
    int kinda = GetKind();

    switch(kinda) {

    case PIECE_QUEEN:
      return 'q';
    case PIECE_KING:
      return 'k';
    case PIECE_KNIGHT:
      return 'n';
    case PIECE_ROOK:
      return 'r';
    case PIECE_PAWN:
      return 'p';
    case PIECE_BISHOP:
      return 'b';
    }
    
    return '-';
  }

  coord_t Piece::getX()
  {
    return x;
  }

  coord_t Piece::getY()
  {
    return y;
  }

  coord_t Piece::getPos()
  {
    return pos;
  }
  
  int Piece::getPosEvaluation()
  {
	return 0;
  }

  void Piece::moveTo(coord_t newpos)
  {
    this->x   = pos2x(newpos);
    this->y   = pos2y(newpos);
    this->pos = newpos;
    //moveCount++;
  }

  void Piece::Rollback(coord_t newpos)
  {
    this->x   = pos2x(newpos);
    this->y   = pos2y(newpos);
    this->pos = newpos;
    //moveCount--;
  }

  bool Piece::IsValidMove_diag(coord_t newpos)
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
        p =  pulchess_board->GetPiece(i,j);
        if( p != NULL )
          return false;
      }
      p = pulchess_board->GetPiece(newpos);
      if( p == NULL || ( p != NULL && IsEnemy(p) )) {
        return true;
      }
    }

    return false;
  }

  bool Piece::IsValidMove_croce(coord_t newpos)
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
        p = pulchess_board->GetPiece(int_x, i);
        if( p != NULL )
          return false;
      }
      p =  pulchess_board->GetPiece(int_newx, int_newy);
      if( p == NULL || ( p != NULL && IsEnemy(p) ) ) {
        return true;
      }
    }


    // ci stiamo muovendo in orizzontale?
    //
    else if( abs(int_y - int_newy) == 0 ) {
      int incr = ( int_x > int_newx ? -1 : 1 );
      for( int i = int_x+incr; i != int_newx; i += incr ) {
        p = pulchess_board->GetPiece(i, int_y);
        if( p != NULL )
          return false;
      }
      p =  pulchess_board->GetPiece(int_newx, int_newy);
      if( p == NULL || ( p != NULL && IsEnemy(p) ) ) {
        return true;
      }
    }

    return false;
  }

string Piece::ToString()
{
  stringstream strs;
  string ris = (colour==WHITE) ?"white":"black";

  strs << ris;
  strs << " ";
  strs << getKindChr();
  strs << getX();
  strs << getY();

  strs >> ris;
  return ris;
}

};
};
