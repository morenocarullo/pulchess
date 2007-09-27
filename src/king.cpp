/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE: King implementation
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
{ /* bianco*/
	0,10,10,10,10,10,10,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
},	
{ /* nero */
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,10,10,10,10,10,10,0
}
};

namespace pulchess { namespace logic {
	
int King::GetKind()
{
	return PIECE_KING;
}

int King::GetRank()
{
    return PIECE_RANK_KING; // king's rank
}

coord_t King::GetValue()
{
    return (coord_t)7 + colour*6;
}

int King::getPosEvaluation()
{
	return posvaltbl[ colour == WHITE ? 0 : 1 ][pos];
}

bool King::IsValidMove(coord_t newpos)
{
    coord_t newx = pos2x(newpos), newy = pos2y(newpos);
    int diffX = int(x) - newx, diffY = int(y) - newy;
	
    // rimanere fermi non va bene!
    if( newpos == pos ) {
		return false;
    }
	
    if( abs(diffX) <= 1 && abs(diffY) <= 1 && OKCOORDS(newpos)) {
		return true;
    }
	
    return false;
}

void King::listMoves(vector<Move *> *mList)
{
    Piece *p;
	
    // mosse normali
    //
    for(int i=-1; i<2; i++) {
		for(int j=-1; j<2; j++) {
			if( !COORDSOK(x+i, y+j) )
				continue;
			
			if(i==0 && j==0)
				continue;
			
			p = pulchess_board->GetPiece(x+i, y+j);
			
			// se esiste una pedina nel posto scelto, ed e' del colore opposto
			// oppure se la casella prescelta e' vuota, allora e' una mossa valida.
			if( p != NULL && IsEnemy(p) ) {
				mList->push_back( new Move( xy2pos(x+i,y+j), pos, 1) );
			}
			else if( p == NULL) {
				mList->push_back( new Move( xy2pos(x+i,y+j), pos, 0) );
			}
		}
    }	
		
    // Kingside castling
    // 
    if( moveCount == 0 &&
		pulchess_board->GetPiece(pos+1) == NULL &&
		pulchess_board->GetPiece(pos+2) == NULL )
	{
		p = pulchess_board->GetPiece(pos+3);
		if( p != NULL &&
		    p->GetKind() == PIECE_ROOK &&
		    p->moveCount == 0 &&
		    !pulchess_board->CanEatThis(pos+1, colour) &&
		    !pulchess_board->CanEatThis(pos+2, colour) )
		{
			mList->push_back( new CastlingMove(KINGSIDE_CASTLING, colour) );
		}
	}
	
	// Queenside castling
	//
	if( moveCount == 0 &&
		pulchess_board->GetPiece(pos-1) == NULL &&
		pulchess_board->GetPiece(pos-2) == NULL &&
		pulchess_board->GetPiece(pos-3) == NULL )
	{
		p = pulchess_board->GetPiece(pos-4);
		if( p != NULL &&
		    p->GetKind() == PIECE_ROOK &&
		    p->moveCount == 0 &&
		    !pulchess_board->CanEatThis(pos-1, colour) &&
		    !pulchess_board->CanEatThis(pos-2, colour) &&
		    !pulchess_board->CanEatThis(pos-3, colour) ) 
		{
			mList->push_back( new CastlingMove(QUEENSIDE_CASTLING, colour) );
		}
	}
}

};
};
