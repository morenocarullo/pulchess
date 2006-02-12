/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE: Move implementations
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
#include <stdio.h>  // C stdio

#define CANCPIECE(X) b->pieceListDel((X));
#define ADDPIECE(X) b->pieceListAdd((X));
#define VIOLATURNO(X) ( (X)->getColour() != b->turn )

using namespace std;

static coord_t char2coord(char c)
{
  if( c >= 'a' && c <= 'h' ) {
    return c - 'a';
  }
  else
    return 255;
}

namespace pulchess { namespace logic {

// Create a CoordsMove, the simplest kind of move used for
// human control purposes.
//
CoordsMove::CoordsMove(string move) /* throw InvalidMoveException */
{
	char cx, cn_x;
	int  y, n_y;
	int rexp = 0;
	
	rexp = sscanf(move.c_str(), "%c%d%c%d", &cx, &y, &cn_x, &n_y );
	
	this->src    = xy2pos( char2coord(cx), y-1 );
	this->dst    = xy2pos( char2coord(cn_x), n_y-1 );
	this->deadPiece = NULL;
	this->promotedSoldier = NULL;
	
	if( !OKCOORDS(dst) || !OKCOORDS(src) ) {
		cout << "Hai richiesto una mossa anomala!" << endl;
		printf("da:%d   a:%d\n", src, dst);
	}
	
	if( rexp != 4 ) {
		throw new InvalidMoveException();
	}
}


Move::Move()
{
}


Move::Move(coord_t newpos, coord_t startpos)
{
    this->dst = newpos;
    this->src = startpos;
    this->deadPiece = NULL;
    this->promotedSoldier = NULL;
	
    if( newpos == startpos ) {
		cerr << "Mossa anomala!" << endl;
    }
	
    if( !OKCOORDS(newpos) || !OKCOORDS(startpos) ) {
		cerr << "Generata mossa anomala!" << endl;
		exit(1);
    }
}

  
Move::~Move() {}


Move * Move::copy()
{
    Move * m = new Move(dst, src);
	
    m->setDeadPiece( this->deadPiece );
    m->setPromotedSoldier( this->promotedSoldier );
    return m;
}


coord_t Move::getSrcIdx()
{
    return src;
}


coord_t Move::getDstIdx()
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

void Move::setPromotedSoldier(Piece * p)
{
    promotedSoldier = p;
}

Piece * Move::getPromotedSoldier()
{
    return promotedSoldier;
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

void Move::play(Board* b)
{
    coord_t
	srcI = getSrcIdx(),
	dstI = getDstIdx();
	
    Piece 
		*src = b->getPiece(srcI),
		*dst = b->getPiece(dstI);
	
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
		
#ifdef DEBUG
		cerr << " Errore: tentata mossa non valida! " << endl;
		printf("da x:%d,y:%d    a x:%d,y:%d\n", pos2x(srcI), pos2y(srcI), pos2x(dstI), pos2y(dstI));
#endif
		
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
    b->setPiece(dstI, src);
    b->setPiece(srcI, NULL);
	
	
    // se il fante arriva in fondo...
    // ...va promosso!
    //
    if( src->getKind() == PIECE_SOLDIER ) {
		if(  (src->getColour() == BLACK && src->getY() == 0) ||
			 (src->getColour() == WHITE && src->getY() == 7) )
		{	 
			// chiede all'utente di scegliere che pezzo inserire
			// al posto del fante appena "promosso".
			Piece *newPiece;
			newPiece = b->getPlayer( src->getColour() )->chooseSoldierPiece();
			ADDPIECE( newPiece );
			b->setPiece( dstI, newPiece );
			newPiece->moveTo( dstI );
			
			// salva le info
			// sul pezzo appena promosso.
			CANCPIECE( src );
			setPromotedSoldier( src );
		}
    }
	
    // incrementiamo il contatore delle mosse ed il turno
    b->moveCount++;
    b->turn = ENEMY(b->turn);
  }

void Move::rewind(Board* b)
{
    coord_t
	srcI = getDstIdx(),
	dstI = getSrcIdx();
	
    Piece
		*src = b->getPiece(srcI),
		*pro = getPromotedSoldier(),
		*ddp = getDeadPiece();
    
    // "spromuove" il fante allo stato originario
    //
    if( pro != NULL ) {
		CANCPIECE(src);
		delete src;
		setPromotedSoldier( NULL );
		
		ADDPIECE( pro );
		b->setPiece( srcI, pro );
    }
	
    // rimette il pezzo alla posizione originaria
    //
    b->getPiece( srcI )->rollback(dstI);
    b->setPiece( dstI, b->getPiece(srcI) );
    b->setPiece( srcI, NULL );
	
	
    // se era stato mangiato un pezzo, lo rimette a posto
    //
    if( ddp != NULL ) {
		b->pieceListAdd( ddp );
		b->setPiece( ddp->getPos(), ddp );
		setDeadPiece( NULL );
    }
	
    // aggiorna mosse e turno
    //
    b->moveCount--;
    b->turn = ENEMY(b->turn);
}

//
// Mossa definitiva. E' stata applicata al gioco, e' possibile eliminare
// i pezzi "mangiati".
//
void Move::commit()
{
    if( deadPiece != NULL ) {
		delete deadPiece;
    }
}

// En passant
EPMove::EPMove(coord_t newpos, coord_t startpos, coord_t eat) : Move(newpos, startpos)
{
	this->dst = newpos;
	this->src = startpos;
	this->eat = eat;
	this->deadPiece = NULL;
	this->promotedSoldier = NULL;
	
	if( newpos == startpos ) {
		cerr << "Mossa anomala!" << endl;
	}
	
	if( !OKCOORDS(newpos) || !OKCOORDS(startpos) ) {
		cerr << "Generata mossa anomala!" << endl;
		exit(1);
	}
}

void EPMove::play(Board *b)
{
    Piece *pEaten;
	
    Move::play(b);
    pEaten = b->getPiece( getEatIdx() );
    setDeadPiece( pEaten );
    CANCPIECE( pEaten );    
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
    m->setPromotedSoldier( this->promotedSoldier );
    return m;
}


RookMove::RookMove(bool rookKind, colour_t colour)
{	
    kind = rookKind;
    pcol = colour;
	
	this->deadPiece = NULL;
	this->promotedSoldier = NULL;
	
	if( rookKind == QUEENSIDE_ROOK ) {
		if( pcol == WHITE ) {
		  this->src = xy2pos(4,0);
		  this->dst = xy2pos(6,0);
		}
		else {
		  this->src = xy2pos(4,7);
		  this->dst = xy2pos(6,7);			
		}
	}
	else {
		if( pcol == WHITE ) {
		  this->src = xy2pos(7,0);
		  this->dst = xy2pos(5,0);
		}
		else {
		  this->src = xy2pos(7,7);
		  this->dst = xy2pos(5,7);			
		}	
	}
}


void RookMove::play(Board *b)
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
	
    b->setPiece( kipos_dst, b->getPiece(kipos_src) );
    b->getPiece( kipos_dst )->moveTo( kipos_dst );
	b->setPiece( kipos_src, NULL );
	
    b->setPiece( rkpos_dst, b->getPiece( rkpos_src ) );
    b->getPiece( rkpos_dst )->moveTo( rkpos_dst );
	b->setPiece( rkpos_src, NULL );
}


// "Disfa" la mossa, torna alla situazione precedente.
//
//////////////////////////////////////////////////////
void RookMove::rewind(Board *b)
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
	
    b->setPiece( kipos_dst, b->getPiece( kipos_src ) );
    b->getPiece( kipos_dst )->moveTo( kipos_dst );
	b->setPiece( kipos_src, NULL );
	
    b->setPiece( rkpos_dst, b->getPiece( rkpos_src ) );
    b->getPiece( rkpos_dst )->moveTo( rkpos_dst );
	b->setPiece( rkpos_src, NULL );
}

Move * RookMove::copy()
{
    return new RookMove(kind, pcol);
}

};
};
