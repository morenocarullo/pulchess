/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Pawn implementation
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
{ /* bianco */
	0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,
	0,2,2,2,2,2,2,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	95,95,95,95,95,95,95,95
},	
{ /* nero */
	95,95,95,95,95,95,95,95,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,2,2,2,2,2,2,0,
	1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0
}
}; 

namespace pulchess { namespace logic {
	
int Pawn::getKind()
{
	return PIECE_SOLDIER;
}

int Pawn::getRank()
{
    return 16; // soldier's rank
}

coord_t Pawn::getValue()
{
    return (coord_t)7 + colour;
}

int Pawn::getPosEvaluation()
{
	return posvaltbl[ colour == WHITE ? 0 : 1 ][pos];
}

bool Pawn::isValidMove(coord_t newpos, Board * b)
{
    Piece *p;
    coord_t newx  = pos2x(newpos), newy = pos2y(newpos);
    int diffX = newx > x ? int(newx) - x : int(x) - newx;
	
    // se stiamo provando a spostare il pezzo fuori dalla scacchiera...
    // ... la mossa non e' valida !
    if( !OKCOORDS(newpos) )
		return false;
	
    // cella di destinazione
    p = b->getPiece(newpos);
	
    // se la destinazione e' una pedina da mangiare (non e' del ns. colore)...
    // ed il "senso" di marcia e' giusto...
    if( p != NULL && isEnemy(p) ) {
		if( getColour() == WHITE && diffX == 1 && newy - y == 1 )
			return true;
		if( getColour() == BLACK && diffX == 1 && y - newy == 1 )
			return true;
    }
    
    // se la destinazione e' vuota e davanti al soldato
    // se e' la prima mossa, possiamo muovere di 2. altrimenti di 1 soltanto.
    if( x-newx == 0 && p == NULL ) {
		// apertura di 2 celle
		if( getColour() == WHITE && getMoveCount() == 0
			&& newy - y == 16 && b->getPiece(newpos-8) == NULL) 
			return true;
		if( getColour() == WHITE && getMoveCount() >= 0 && newy - y == 8)
			return true;
		if( getColour() == BLACK && getMoveCount() == 0
			&& y - newy == 16 && b->getPiece(newpos+8) == NULL)
			return true;
		if( getColour() == BLACK && getMoveCount() >= 0 && y - newy == 8)
			return true;
    }
	
    // se si tratta di un en passant...
    if( ISRELCELL(5) ) {
		Piece *ped = b->getPiece( newpos - getColour()*8 );
		if(  (newx == x+1 || newx == x-1)  &&
			 (newy == y+getColour())       &&
			 (p == NULL)                   &&
			 (ped != NULL && isEnemy(ped) && ped->getMoveCount() == 1) )
		{
			return true;
		}
    }
	
    // altrimenti...
    return false;
}

list<Move *> * Pawn::listMoves(Board* b, list<Move *> *mList)
{
    Piece      *p = NULL, *op = NULL;
	
    // 1 - possiamo mangiare qualcuno ?
    p = b->getPiece(x+1, y+getColour());
    if( p!=NULL && isEnemy(p) ) {
		mList->push_front( new Move( xy2pos( x+1, y+getColour() ), pos) );
    }
    p = b->getPiece(x-1, y+getColour());
    if( p!=NULL && isEnemy(p) ) {
		mList->push_front( new Move( xy2pos( x-1, y+getColour() ), pos) );
    }
	
    // 2 - possiamo mangiare con l'en passant ?
    //   o  se noi siamo nella quinta casella (x5)
    //   o  di fianco a noi abbiamo un pedone la cui ultima mossa e' stata
    //      di due caselle, allora possiamo mangiarlo muovendoci in diagonale
    //      +1,+1 oppure -1,-1
    //
    if( ISRELCELL(5) ) {
		Piece *dst;
		for( int i=x-1; i<x+2; i+=2 ) {
			if( !COORDSOK( i, y ) )
				continue;
			// 
			// en passant e' valido se:
			// o  c'e' un pezzo nemico al nostro fianco alla prima mossa
			// o  la cella di destinazione e' libera
			//
			dst = b->getPiece(i, y);
			if( dst!=NULL && dst->getMoveCount() == 1 &&
				isEnemy(dst) &&
				b->getPiece(i, y+getColour()) == NULL )
			{
				mList->push_front( new EPMove( xy2pos(i, y+getColour()), pos, xy2pos(i, y)) );
			}
		}
    }
	
    // 3 - possiamo muoverci in avanti di 1?
    p = b->getPiece(x, y + 1*getColour());
    if( p == NULL ) {		
  		Move * m  = new Move( xy2pos(x, y + 1*getColour()), pos);		
  		//
  		// euristica: se si tratta di una promozione,
  		//			  la mossa e' sicuramente migliore.
  		ISRELCELL(8) ? mList->push_front(m) : mList->push_back(m);
    }
    
    // 4 - ... e di 2?
    p  = b->getPiece(x, y + 2*getColour());
    op = b->getPiece(x, y + 1*getColour());
    if( p == NULL && op == NULL && getMoveCount() == 0) {
  		mList->push_back( new Move( xy2pos(x, y+2*getColour()), pos) );
    }
	
    return mList;
}

};
};