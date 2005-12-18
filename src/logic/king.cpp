/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: King implementation
 *
 * Created on 15-lug-2005
 * $Id$
 */
#include "logic/stdheader.h"

static byte posvaltbl[2][64] = 
{
{
	9,9,9,9,9,9,9,9,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
},	
{
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	9,9,9,9,9,9,9,9
}
};

namespace pulchess { namespace logic {
	
int King::getKind()
{
		return PIECE_KING;
}

int King::getRank()
{
    return 200; // king's rank
}

byte King::getValue()
{
    return (byte)7 + colour*6;
}

byte King::getPosEvaluation()
{
	return posvaltbl[ colour == WHITE ? 0 : 1 ][pos];
}

bool King::isValidMove(byte newpos, Board * b)
{
    int newx = pos2x(newpos), newy = pos2y(newpos);
    int diffX = x - newx, diffY = y - newy;
	
    // rimanere fermi non va bene!
    if( newpos == pos ) {
		return false;
    }
	
    // controllare che la mossa non metta il re in check!
    if( abs(diffX) <= 1 && abs(diffY) <= 1 && OKCOORDS(newpos)) {
		return true;
    }
	
    return false;
}

list<Move *> * King::listMoves(Board* b)
{
    list<Move *> * mList = new list<Move *>();
    Piece *p;
	
    // mosse normali
    //
    for(int i=-1; i<2; i++) {
		for(int j=-1; j<2; j++) {
			if( !COORDSOK(x+i, y+j) )
				continue;
			
			if(i==0 && j==0)
				continue;
			
			p = b->getPiece(x+i, y+j);
			
			// se esiste una pedina nel posto scelto, ed e' del colore opposto
			// oppure se la casella prescelta e' vuota, allora e' una mossa valida.
			if( p != NULL && isEnemy(p) ) {
				mList->push_front( new Move( xy2pos(x+i,y+j), pos) );
			}
			else if( p == NULL) {
				mList->push_back( new Move( xy2pos(x+i,y+j), pos) );
			}
		}
    }	
		
    // Kingside castling
    // 
    if( getMoveCount() == 0 &&
		b->getPiece(pos+1) == NULL &&
		b->getPiece(pos+2) == NULL )
	{
		p = b->getPiece(pos+3);
		if( p != NULL &&
		    p->getKind() == PIECE_TOWER &&
		    p->getMoveCount() == 0 &&
		    !b->canEatThis(pos+1, ENEMY(colour)) &&
		    !b->canEatThis(pos+2, ENEMY(colour)) )
		{
			mList->push_back( new RookMove(KINGSIDE_ROOK, colour) );
		}
	}
	
	// Queenside castling
	//
	if( getMoveCount() == 0 &&
		b->getPiece(pos-1) == NULL &&
		b->getPiece(pos-2) == NULL &&
		b->getPiece(pos-3) == NULL )
	{
		p = b->getPiece(pos-4);
		if( p != NULL &
		    p->getKind() == PIECE_TOWER &&
		    p->getMoveCount() == 0 &&
		    !b->canEatThis(pos-1, ENEMY(colour)) &&
		    !b->canEatThis(pos-2, ENEMY(colour)) &&
		    !b->canEatThis(pos-3, ENEMY(colour)) ) 
		{
			mList->push_back( new RookMove(QUEENSIDE_ROOK, colour) );
		}
	}
    
    return mList;
}

};
};
