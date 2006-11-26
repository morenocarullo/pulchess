/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE: Move implementations
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
#include <stdio.h>  // C stdio

#define CANCPIECE(X) b->PieceListDel((X));
#define ADDPIECE(X) b->PieceListAdd((X));
#define VIOLATURNO(X) ( (X)->GetColour() != b->turn )

using namespace std;

namespace pulchess { namespace logic {

//
// Convert a char coord into coord_t value
static inline coord_t char2coord(char c)
{
  return (c >= 'a' && c <= 'h') ? c - 'a' : 255;
}

//
// Convert coord_t value into char
static inline char coord2char(coord_t posx)
{
   return 'a' + posx;
}

static inline char coordn2char(coord_t posy)
{
   return '1' + posy;
}

//
// Create a CoordsMove, the simplest kind of move used for
// human control purposes.
CoordsMove::CoordsMove(string move) /* throw InvalidMoveException */
{
	char cx, cn_x;
	int  y, n_y;
	int rexp = 0;
	
	rexp = sscanf(move.c_str(), "%c%d%c%d", &cx, &y, &cn_x, &n_y );
	
	this->src    = xy2pos( char2coord(cx), y-1 );
	this->dst    = xy2pos( char2coord(cn_x), n_y-1 );
	this->deadPiece = NULL;
	this->promotedPawn = NULL;
	
	if( !OKCOORDS(dst) || !OKCOORDS(src) ) {
		pulchess_error( "Hai richiesto una mossa anomala!" );
		printf("da:%d   a:%d\n", src, dst);
		throw new InvalidMoveException();
	}
	
	if( rexp != 4 ) {
		throw new InvalidMoveException();
	}
}

//
// Default ctor
//
Move::Move()
{
}

//
// Ctor with specified starting and ending place
//
Move::Move(coord_t newpos, coord_t startpos)
{
    this->dst = newpos;
    this->src = startpos;
    this->deadPiece = NULL;
    this->promotedPawn = NULL;
	
    if( newpos == startpos ) {
		cerr << "Mossa anomala!" << endl;
    }
	
    if( !OKCOORDS(newpos) || !OKCOORDS(startpos) ) {
		cerr << "Generata mossa anomala!" << endl;
		cerr << "startpos x: " << pos2x(startpos) << " startpos y: " << pos2y(startpos) << endl;
		cerr << "newpos x:  " << pos2x(newpos) << " newpos y:  " << pos2y(newpos) << endl;
    }
}

//
// Default D'stor
//
Move::~Move() {}

//
// Get a string representation of the move
//
string Move::toString()
{
	string * res, res2;
	char buff[5];
	
	buff[0] = coord2char(pos2x(src));
	buff[1] = coordn2char(pos2y(src));
	buff[2] = coord2char(pos2x(dst));
	buff[3] = coordn2char(pos2y(dst));
	buff[4] = '\0';
	
	res = new string(buff);
	res2 = *res;
	delete res;
	return res2;
}

//
// Equal operator
//
bool Move::operator== (Move &a)
{
	return (GetSrcIdx() == a.GetSrcIdx() && GetDstIdx() == a.GetDstIdx());
}

Move * Move::copy()
{
    Move * m = new Move(dst, src);
	
    m->setDeadPiece( this->deadPiece );
    m->setPromotedPawn( this->promotedPawn );
    return m;
}


coord_t Move::GetSrcIdx()
{
    return src;
}


coord_t Move::GetDstIdx()
{
    return dst;
}

//
//
//////////////////////////////////
void Move::setDeadPiece(Piece * p)
{
    deadPiece = p;
}


//
//
////////////////////////////
Piece * Move::getDeadPiece()
{
    return deadPiece;
}

void Move::setPromotedPawn(Piece * p)
{
    promotedPawn = p;
}

Piece * Move::getPromotedPawn()
{
    return promotedPawn;
}

coord_t Move::getSourceX()
{
    return pos2x(src);
}

coord_t Move::getSourceY()
{
    return pos2y(src);
}

coord_t Move::getX()
{
    return pos2x(dst);             
}

coord_t Move::getY()
{
    return pos2y(dst);             
}  

int Move::Play(Board* b)
{
    coord_t
	srcI = GetSrcIdx(),
	dstI = GetDstIdx();
	
    Piece 
		*src = b->GetPiece(srcI),
		*dst = b->GetPiece(dstI);
	
    // 
    // Precondizioni :
    //  1) le coordinate sorgenti devono essere ok (gia' vero in Move)
    //  2) le coordinate di destinazione devono essere ok (gia' vero in Move)
    //
    //  3) il pezzo scelto deve essere valido
    //  4) deve essere del colore giusto
    //  5) non deve essere una mossa da A ad A
    //  6) la mossa deve essere valida per il pezzo scelto*
    //     *se la mossa e' generata, sappiamo gia' che e' valida
    //
    if( src == NULL      ||
		VIOLATURNO(src)  ||
		srcI == dstI )
	{
		pulchess_error( "tentata mossa non valida: " << this->toString() );
		throw new InvalidMoveException("Mossa non valida!");
    }
	
    // abbiamo mangiato ?
    //
    if( dst != NULL ) {
		setDeadPiece( dst );
		CANCPIECE( dst );
    }
	
    // spostiamo il pezzo
    //
    src->moveTo( dstI );
    b->SetPiece(dstI, src);
    b->SetPiece(srcI, NULL);
	
	
    // se il fante arriva in fondo...
    // ...va promosso!
    //
    if( src->GetKind() == PIECE_PAWN ) {
		if(  (src->GetColour() == BLACK && src->getY() == 0) ||
			 (src->GetColour() == WHITE && src->getY() == 7) )
		{	 
			// chiede all'utente di scegliere che pezzo inserire
			// al posto del fante appena "promosso".
			Piece *newPiece;
			newPiece = b->GetPlayer( src->GetColour() )->ChoosePawnPiece();
			ADDPIECE( newPiece );
			b->SetPiece( dstI, newPiece );
			newPiece->moveTo( dstI );
			
			// salva le info
			// sul pezzo appena promosso.
			CANCPIECE( src );
			setPromotedPawn( src );
		}
    }
	
    // incrementiamo il contatore delle mosse ed il turno
    b->moveCount++;
    b->turn = ENEMY(b->turn);

    // return 1 if we ate a piece
    return getDeadPiece() != NULL ? getDeadPiece()->GetRank() : 0;
  }

void Move::Rewind(Board* b)
{
    coord_t
	srcI = GetDstIdx(),
	dstI = GetSrcIdx();
	
    Piece
		*src = b->GetPiece(srcI),
		*pro = getPromotedPawn(),
		*ddp = getDeadPiece();
    
    // "spromuove" il fante allo stato originario
    //
    if( pro != NULL ) {
		CANCPIECE(src);
		delete src;
		setPromotedPawn( NULL );
		
		ADDPIECE( pro );
		b->SetPiece( srcI, pro );
    }
	
    // rimette il pezzo alla posizione originaria
    //
    b->GetPiece( srcI )->rollback(dstI);
    b->SetPiece( dstI, b->GetPiece(srcI) );
    b->SetPiece( srcI, NULL );
	
	
    // se era stato mangiato un pezzo, lo rimette a posto
    //
    if( ddp != NULL ) {
		b->PieceListAdd( ddp );
		b->SetPiece( ddp->getPos(), ddp );
		setDeadPiece( NULL );
    }
	
    // aggiorna mosse e turno
    //
    b->moveCount--;
    b->turn = ENEMY(b->turn);
}

//
// En passant move ctor
//
EPMove::EPMove(coord_t newpos, coord_t startpos, coord_t eat) : Move(newpos, startpos)
{
	this->dst = newpos;
	this->src = startpos;
	this->eat = eat;
	this->deadPiece = NULL;
	this->promotedPawn = NULL;
	
	if( newpos == startpos ) {
		pulchess_error( "Mossa anomala!" );
	}
	
	if( !OKCOORDS(newpos) || !OKCOORDS(startpos) ) {
		pulchess_error( "Generata mossa anomala!" );
		exit(1);
	}
}

int EPMove::Play(Board *b)
{
    Piece *pEaten = NULL;
    pEaten = b->GetPiece( getEatIdx() );
    Move::Play(b);
    setDeadPiece( pEaten );
    CANCPIECE( pEaten );
    b->SetPiece( getEatIdx(), NULL);

    return 1; // special effect: pawn eats pawn    
}

coord_t EPMove::getEatIdx()
{
    return eat;
}

coord_t EPMove::getEatX()
{
    return pos2x(eat);
}

coord_t EPMove::getEatY()
{
    return pos2y(eat);
}

Move * EPMove::copy()
{
    Move * m = new EPMove(dst, src, eat);
	
    m->setDeadPiece( this->deadPiece );
    m->setPromotedPawn( this->promotedPawn );
    return m;
}


RookMove::RookMove(bool rookKind, colour_t colour)
{	
    kind = rookKind;
    pcol = colour;
	
	this->deadPiece = NULL;
	this->promotedPawn = NULL;
	
	if( rookKind == QUEENSIDE_ROOK ) {
		if( pcol == WHITE ) {
		  this->src = xy2pos(4,0);
		  this->dst = xy2pos(1,0);
		}
		else {
		  this->src = xy2pos(4,7);
		  this->dst = xy2pos(1,7);			
		}
	}
	else {
		if( pcol == WHITE ) {
		  this->src = xy2pos(4,0);
		  this->dst = xy2pos(6,0);
		}
		else {
		  this->src = xy2pos(4,7);
		  this->dst = xy2pos(6,7);			
		}	
	}
}


int RookMove::Play(Board *b)
{
    coord_t rkpos_src, kipos_src, rkpos_dst, kipos_dst;
	
    if( kind == KINGSIDE_ROOK ) {
		if(pcol == WHITE) {
			kipos_src = xy2pos(4,0);
			kipos_dst = xy2pos(6,0);
			rkpos_src = xy2pos(7,0);
			rkpos_dst = xy2pos(5,0);
		}
		else {
			kipos_src = xy2pos(4,7);
			kipos_dst = xy2pos(6,7);
			rkpos_src = xy2pos(7,7);
			rkpos_dst = xy2pos(5,7);
		}
    }
    else {
		if( pcol == WHITE ) {
			kipos_src = xy2pos(4,0);
			kipos_dst = xy2pos(1,0);
			rkpos_src = xy2pos(0,0);
			rkpos_dst = xy2pos(2,0);
		}
		else {
			kipos_src = xy2pos(4,7);
			kipos_dst = xy2pos(1,7);
			rkpos_src = xy2pos(0,7);
			rkpos_dst = xy2pos(2,7);
		}
    }
	
    b->SetPiece( kipos_dst, b->GetPiece(kipos_src) );
    b->GetPiece( kipos_dst )->moveTo( kipos_dst );
	b->SetPiece( kipos_src, NULL );
	
    b->SetPiece( rkpos_dst, b->GetPiece( rkpos_src ) );
    b->GetPiece( rkpos_dst )->moveTo( rkpos_dst );
	b->SetPiece( rkpos_src, NULL );
	
	return 0; // no special effect
}


// "Disfa" la mossa, torna alla situazione precedente.
//
//////////////////////////////////////////////////////
void RookMove::Rewind(Board *b)
{
    coord_t rkpos_src, kipos_src, rkpos_dst, kipos_dst;
	
    if( kind == KINGSIDE_ROOK ) {
		if( pcol == WHITE) {
			kipos_src = xy2pos(6,0);
			kipos_dst = xy2pos(4,0);
			rkpos_src = xy2pos(5,0);
			rkpos_dst = xy2pos(7,0);
		}
		else {
			kipos_src = xy2pos(6,7);
			kipos_dst = xy2pos(4,7);
			rkpos_src = xy2pos(5,7);
			rkpos_dst = xy2pos(7,7);
		}
    }
    else {
		if( pcol == WHITE ) {
			kipos_src = xy2pos(1,0);
			kipos_dst = xy2pos(4,0);
			rkpos_src = xy2pos(2,0);
			rkpos_dst = xy2pos(0,0);
		}
		else {
			kipos_src = xy2pos(1,7);
			kipos_dst = xy2pos(4,7);
			rkpos_src = xy2pos(2,7);
			rkpos_dst = xy2pos(0,7);
		}
    }
	
    b->SetPiece( kipos_dst, b->GetPiece( kipos_src ) );
    b->GetPiece( kipos_dst )->moveTo( kipos_dst );
	b->SetPiece( kipos_src, NULL );
	
    b->SetPiece( rkpos_dst, b->GetPiece( rkpos_src ) );
    b->GetPiece( rkpos_dst )->moveTo( rkpos_dst );
	b->SetPiece( rkpos_src, NULL );
}

Move * RookMove::copy()
{
    return new RookMove(kind, pcol);
}

};
};
