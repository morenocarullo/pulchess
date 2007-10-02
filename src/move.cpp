/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
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

#define CANCPIECE(X) pulchess_board->PieceListDel((X));
#define ADDPIECE(X) pulchess_board->PieceListAdd((X));
#define VIOLATURNO(X) ( (X)->colour != pulchess_board->turn )

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
Move::Move(coord_t newpos, coord_t startpos, int rating)
{
    this->dst = newpos;
    this->src = startpos;
    this->deadPiece = NULL;
    this->promotedPawn = NULL;
	this->rating = rating;
	
    if( newpos == startpos ) {
		pulchess_error( "Mossa anomala!" );
    }
	
    if( !OKCOORDS(newpos) || !OKCOORDS(startpos) ) {
		pulchess_error( "Generata mossa anomala!" );
		pulchess_error( "startpos x: " << pos2x(startpos) << " startpos y: " << pos2y(startpos) );
		pulchess_error( "newpos x:  " << pos2x(newpos) << " newpos y:  " << pos2y(newpos) );
    }
}

//
// Default D'stor
//
Move::~Move() {}

//
// Move factory: given a text command it instantiates the correspoinding Move.
//    TODO: refactor here the code needed by PAWN PROMOTION !
//
Move * Move::GetMoveFactory(string moveCmd, colour_t colour)
{
	CoordsMove *coords;
	vector<Move *> mList;
	vector<Move *>::iterator mListIt;
	Piece      *srcp;
	
    // interpreta la mossa
	try {
	   coords = new CoordsMove(moveCmd);
    }
    catch(InvalidMoveException *e)
    {
       pulchess_info("la mossa specificata non e' valida");
	   return NULL;
	}
	
	srcp = pulchess_board->GetPiece( coords->GetSrcIdx() );
	
	// errore: qui non c'e' nessun pezzo
	if( srcp == NULL ) return NULL;
	
	// errore: non possiamo muovere pezzi di altri!
	if( srcp->colour != colour ) return NULL;
	
	srcp->listMoves(&mList);
	for(mListIt = mList.begin(); mListIt != mList.end(); mListIt++) {
		if( (*mListIt)->GetSrcIdx() == coords->GetSrcIdx() &&
			(*mListIt)->GetDstIdx() == coords->GetDstIdx() ) {
			Move * copy = (*mListIt)->copy();
			moveListDestroy(&mList);
			return copy;
		}
	}
	
	pulchess_debug( "Move::" << __func__ << " -- la mossa specificata non"
                    << " e' stata trovata tra quelle valide: " << coords->toString() );

	for(mListIt = mList.begin(); mListIt != mList.end(); mListIt++) {
		pulchess_debug( "\tmossa disp: " << (*mListIt)->toString() );
	}
	
	moveListDestroy(&mList);
	return NULL;
}

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

//
// Less operator
//
bool operator< (const Move &a, const Move &b)
{
	return a.rating > b.rating;
}

//
// Copy this move. TODO: replace with standard clone/copy
//
Move * Move::copy()
{
    Move * m = new Move(dst, src, rating);
	
    m->deadPiece      = this->deadPiece;
    m->promotedPawn   = this->promotedPawn;
    m->enpassant      = this->enpassant;
	m->fiftyMovesRule = this->fiftyMovesRule;
    
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

int Move::Play()
{
    coord_t
	srcI = GetSrcIdx(),
	dstI = GetDstIdx();
	
    Piece 
		*src = pulchess_board->GetPiece(srcI),
		*dst = pulchess_board->GetPiece(dstI);
	
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
#ifdef DEBUG
    if( src == NULL  ||
	VIOLATURNO(src)  ||
	srcI == dstI )
    {
		pulchess_error( "tentata mossa non valida: " << this->toString() );
		throw new InvalidMoveException("Mossa non valida!");
    }
#endif

	// salvataggio stato 50 mosse
	fiftyMovesRule = pulchess_board->fiftyMovesRule;
	
    // salviamo il flag per l'enpassant
    enpassant = pulchess_board->enpassant;
    pulchess_board->enpassant = NO_POSITION;
	
    // abbiamo mangiato ?
    if( dst != NULL ) {   
		deadPiece = dst;
		CANCPIECE( dst );
		pulchess_board->fiftyMovesRule = 0;
    }
	else { // altrimenti, proviamo ad incrementare il contatore 50mosse.
		pulchess_board->fiftyMovesRule++;
	}
	
    // spostiamo il pezzo
    //
    src->moveTo( dstI );
    pulchess_board->SetPiece(dstI, src);
    pulchess_board->SetPiece(srcI, NULL);
	
	// casi per il pedone
    //
    if( src->GetKind() == PIECE_PAWN ) {
	
		// se viene mosso un pedone si azzera la 50 move rule
		pulchess_board->fiftyMovesRule = 0;
	
		// promozione del pedone
		if(  (src->colour == BLACK && src->getY() == 0) ||
			 (src->colour == WHITE && src->getY() == 7) )
		{	 
			// chiede all'utente di scegliere che pezzo inserire
			// al posto del fante appena "promosso".
			Piece *newPiece;
			newPiece = pulchess_board->GetPlayer( src->colour )->ChoosePawnPiece();
			ADDPIECE( newPiece );
			pulchess_board->SetPiece( dstI, newPiece );
			newPiece->moveTo( dstI );
			
			// salva le info
			// sul pezzo appena promosso.
			CANCPIECE( src );
			promotedPawn = src;
		}
    }
	
    // incrementiamo il contatore delle mosse ed il turno
    pulchess_board->moveCount++;
    pulchess_board->turn = ENEMY(pulchess_board->turn);
    
    // se e' un pedone, ev. settiamo il flag di enpassant
    if( src->GetKind() == PIECE_PAWN && abs((int)srcI - dstI) == 16 ) {
       enpassant = dstI;
    }

    // return rating
    return rating;
}

void Move::Rewind()
{
    coord_t
	srcI = GetDstIdx(),
	dstI = GetSrcIdx();
	
    Piece
		*src = pulchess_board->GetPiece(srcI),
		*pro = promotedPawn,
		*ddp = deadPiece;
    
    // "spromuove" il fante allo stato originario
    //
    if( pro != NULL ) {
		CANCPIECE(src);
		delete src;
		promotedPawn = NULL;
		
		ADDPIECE( pro );
		pulchess_board->SetPiece( srcI, pro );
    }
	
    // rimette il pezzo alla posizione originaria
    //
    pulchess_board->GetPiece( srcI )->Rollback(dstI);
    pulchess_board->SetPiece( dstI, pulchess_board->GetPiece(srcI) );
    pulchess_board->SetPiece( srcI, NULL );
	
	
    // se era stato mangiato un pezzo, lo rimette a posto
    //
    if( ddp != NULL ) {
		pulchess_board->PieceListAdd( ddp );
		pulchess_board->SetPiece( ddp->getPos(), ddp );
		deadPiece = NULL;
    }
	
    // aggiorna mosse e turno
    pulchess_board->moveCount--;
    pulchess_board->turn = ENEMY(pulchess_board->turn);
    
    // reimposta en passant flag
    pulchess_board->enpassant = enpassant;

	// reimposta 50 move rule
	pulchess_board->fiftyMovesRule = fiftyMovesRule;
}

//
// En passant move ctor
//
EPMove::EPMove(coord_t newpos, coord_t startpos, coord_t eat) : Move(newpos, startpos, 20)
{
	this->dst = newpos;
	this->src = startpos;
	this->eat = eat;
	this->deadPiece = NULL;
	this->promotedPawn = NULL;

#ifdef DEBUG
	if( newpos == startpos ) {
		pulchess_error( "Mossa anomala!" );
	}
	
	if( !OKCOORDS(newpos) || !OKCOORDS(startpos) ) {
		pulchess_error( "Generata mossa anomala!" );
		exit(1);
	}
#endif
}

int EPMove::Play()
{
    Piece *pEaten = NULL;
    pEaten = pulchess_board->GetPiece( getEatIdx() );
    Move::Play();
    deadPiece = pEaten;
    CANCPIECE( pEaten );
    pulchess_board->SetPiece( getEatIdx(), NULL);
    
	//  ATTENZIONE
	//  contatori e stato aggiornati gia' nella Move::Play() !!!
	//
    return 1;    
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
	
    m->deadPiece = this->deadPiece;
    m->promotedPawn = this->promotedPawn;
    m->enpassant = this->enpassant;
	m->fiftyMovesRule = this->fiftyMovesRule;
        
    return m;
}


CastlingMove::CastlingMove(bool rookKind, colour_t colour)
{	
    kind = rookKind;
    pcol = colour;
	
	this->deadPiece = NULL;
	this->promotedPawn = NULL;
	
	if( rookKind == QUEENSIDE_CASTLING ) {
		if( pcol == WHITE ) {
		  this->src = xy2pos(4,0);
		  this->dst = xy2pos(2,0);
		}
		else {
		  this->src = xy2pos(4,7);
		  this->dst = xy2pos(2,7);			
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


int CastlingMove::Play()
{
    coord_t rkpos_src, kipos_src, rkpos_dst, kipos_dst;
	
    if( kind == KINGSIDE_CASTLING ) {
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
			kipos_dst = xy2pos(2,0);
			rkpos_src = xy2pos(0,0);
			rkpos_dst = xy2pos(3,0);
		}
		else {
			kipos_src = xy2pos(4,7);
			kipos_dst = xy2pos(2,7);
			rkpos_src = xy2pos(0,7);
			rkpos_dst = xy2pos(3,7);
		}
    }
	
    pulchess_board->SetPiece( kipos_dst, pulchess_board->GetPiece(kipos_src) );
    pulchess_board->GetPiece( kipos_dst )->moveTo( kipos_dst );
	pulchess_board->SetPiece( kipos_src, NULL );
	
    pulchess_board->SetPiece( rkpos_dst, pulchess_board->GetPiece( rkpos_src ) );
    pulchess_board->GetPiece( rkpos_dst )->moveTo( rkpos_dst );
	pulchess_board->SetPiece( rkpos_src, NULL );
	
    // aggiorna mosse e turno
    pulchess_board->moveCount++;
    pulchess_board->turn = ENEMY(pulchess_board->turn);
	
    // resettiamo il flag per l'enpassant
    enpassant = pulchess_board->enpassant;
    pulchess_board->enpassant = NO_POSITION;

	// regola delle 50 mosse, incrementiamo perche' l'arrocco non e' una cattura
	fiftyMovesRule = pulchess_board->fiftyMovesRule;
	pulchess_board->fiftyMovesRule++;
	
	return 0; // no special effect
}

//
// "Disfa" la mossa, torna alla situazione precedente.
//
void CastlingMove::Rewind()
{
    coord_t rkpos_src, kipos_src, rkpos_dst, kipos_dst;
	
    if( kind == KINGSIDE_CASTLING ) { // Kingside
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
    else { // Queenside
		if( pcol == WHITE ) {
			kipos_src = xy2pos(2,0);
			kipos_dst = xy2pos(4,0);
			rkpos_src = xy2pos(3,0);
			rkpos_dst = xy2pos(0,0);
		}
		else {
			kipos_src = xy2pos(2,7);
			kipos_dst = xy2pos(4,7);
			rkpos_src = xy2pos(3,7);
			rkpos_dst = xy2pos(0,7);
		}
    }
	
    pulchess_board->SetPiece( kipos_dst, pulchess_board->GetPiece( kipos_src ) );
    pulchess_board->GetPiece( kipos_dst )->Rollback( kipos_dst );
    pulchess_board->SetPiece( kipos_src, NULL );
	
    pulchess_board->SetPiece( rkpos_dst, pulchess_board->GetPiece( rkpos_src ) );
    pulchess_board->GetPiece( rkpos_dst )->Rollback( rkpos_dst );
    pulchess_board->SetPiece( rkpos_src, NULL );
	
    // aggiorna mosse e turno
    pulchess_board->moveCount--;
    pulchess_board->turn = ENEMY(pulchess_board->turn);	
	
    // aggiorna en passant flag
    pulchess_board->enpassant = enpassant;

	// flag 50 mosse
	pulchess_board->fiftyMovesRule = fiftyMovesRule;
}

Move * CastlingMove::copy()
{
  Move * m = new CastlingMove(kind, pcol);
  
  m->deadPiece = this->deadPiece;
  m->promotedPawn = this->promotedPawn;
  m->enpassant = this->enpassant;
  m->fiftyMovesRule = this->fiftyMovesRule;
 
  return m;
}

};
};
