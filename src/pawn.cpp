/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Pawn implementation
 *
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
 *
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
	0, 0, 0, 0, 0, 0, 0, 0,
	95,95,95,95,95,95,95,95,
	95, 95, 95, 95, 95, 95, 95, 95
},	
{ /* nero */
	95, 95, 95, 95, 95, 95, 95, 95,
	95,95,95,95,95,95,95,95,
	0, 0, 0, 0, 0, 0, 0, 0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,2,2,2,2,2,2,0,
	1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0
}
}; 

namespace pulchess { namespace logic {
	
int Pawn::GetKind()
{
	return PIECE_PAWN;
}

int Pawn::GetRank()
{
    return PIECE_RANK_PAWN;
}

coord_t Pawn::GetValue()
{
    return (coord_t)7 + colour;
}

int Pawn::getPosEvaluation()
{
	return posvaltbl[ colour == WHITE ? 0 : 1 ][pos];
}

bool Pawn::IsValidMove(coord_t newpos)
{
    Piece *p;
    coord_t newx  = pos2x(newpos), newy = pos2y(newpos);
    int diffX = newx > x ? int(newx) - x : int(x) - newx;
	
    // se stiamo provando a spostare il pezzo fuori dalla scacchiera...
    // ... la mossa non e' valida !
    if( !OKCOORDS(newpos) )
		return false;
	
    // cella di destinazione
    p = pulchess_board->GetPiece(newpos);
	
    // se la destinazione e' una pedina da mangiare (non e' del ns. colore)...
    // ed il "senso" di marcia e' giusto...
    if( p != NULL && IsEnemy(p) ) {
		if( colour == WHITE && diffX == 1 && newy - y == 1 )
			return true;
		if( colour == BLACK && diffX == 1 && y - newy == 1 )
			return true;
    }
    
    // se la destinazione e' vuota e davanti al soldato
    // se e' la prima mossa, possiamo muovere di 2. altrimenti di 1 soltanto.
    if( x-newx == 0 && p == NULL ) {
		// apertura di 2 celle
		if( colour == WHITE && moveCount == 0
			&& newy - y == 16 && pulchess_board->GetPiece(newpos-8) == NULL) 
			return true;
		if( colour == WHITE && newy - y == 8)
			return true;
		if( colour == BLACK && moveCount == 0
			&& y - newy == 16 && pulchess_board->GetPiece(newpos+8) == NULL)
			return true;
		if( colour == BLACK && y - newy == 8)
			return true;
    }
	
    // se si tratta di un en passant...
    if( ISRELCELL(5) ) {
		Piece *ped = pulchess_board->GetPiece( newpos - colour*8 );
		if(  (newx == x+1 || newx == x-1)  &&
			 (newy == y+colour)       &&
			 (p == NULL)                   &&
			 (ped != NULL && IsEnemy(ped) && pulchess_board->enpassant == newpos - colour*8) )
		{
			return true;
		}
    }
	
    // altrimenti...
    return false;
}

void Pawn::listMoves(vector<Move *> *mList)
{
    Piece      *p = NULL, *op = NULL;

    int rating = 0;
	
    // 1 - possiamo mangiare qualcuno ?
    p = pulchess_board->GetPiece(x+1, y+colour);
    if( p!=NULL && IsEnemy(p) ) {
	    rating = PULCHESS_RAT_PAWNEATS * p->GetRank();
		mList->push_back( new Move( xy2pos( x+1, y+colour ), pos, rating) );
    }
    p = pulchess_board->GetPiece(x-1, y+colour);
    if( p!=NULL && IsEnemy(p) ) {
	    rating = PULCHESS_RAT_PAWNEATS * p->GetRank();
		mList->push_back( new Move( xy2pos( x-1, y+colour ), pos, rating) );
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
			// OCCHIO: va fatto subito quando si presenta l'occasione, poi non e' piu' valido.
			//
			dst = pulchess_board->GetPiece(i, y);
			if( dst!=NULL && pulchess_board->enpassant == xy2pos(i,y) &&
				IsEnemy(dst) &&
				pulchess_board->GetPiece(i, y+colour) == NULL )
			{
				mList->push_back( new EPMove( xy2pos(i, y+colour), pos, xy2pos(i, y)) );
			}
		}
    }
	
    // 3 - possiamo muoverci in avanti di 1?
    p = pulchess_board->GetPiece(x, y + 1*colour);
    if( p == NULL ) {
	    rating = ISRELCELL(8) ? PULCHESS_RAT_PAWNPROM : 0;
  		Move * m  = new Move( xy2pos(x, y + 1*colour), pos, PULCHESS_RAT_STD);		
		mList->push_back(m);
    }
    
    // 4 - ... e di 2?
    p  = pulchess_board->GetPiece(x, y + 2*colour);
    op = pulchess_board->GetPiece(x, y + 1*colour);
    if( p == NULL && op == NULL && moveCount == 0) {
  		mList->push_back( new Move( xy2pos(x, y+2*colour), pos, PULCHESS_RAT_STD) );
    }
}

};
};
