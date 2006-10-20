/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Board and BoardValue implementation
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

#define piece_at(X,Y)  _map[ (Y)*8 + (X) ]
#define piece_dr(I)    _map[ (I) ]

#define CANCPIECE(X) { (X)->getColour() == WHITE ? whiteList.remove(X) : blackList.remove(X); }
#define ADDPIECE(X) { (X)->getColour() == WHITE ? whiteList.push_back(X) : blackList.push_back(X); }
#define VIOLATURNO(X) ( (X)->getColour() != turn )

namespace pulchess { namespace logic {

//
// Board ctor
//	
Board::Board(PlayerIF * white, PlayerIF * black)
{
		// assign players
		_whitePlayer = white;
		_blackPlayer = black;
		
		if(_whitePlayer != NULL && _blackPlayer != NULL ) {
			_whitePlayer->setBoard(this);
			_blackPlayer->setBoard(this);
		}
		
		// il bianco inizia per primo!
		turn = WHITE;
		
		// init board
		for(int i=0; i<64; i++) 
		{
			_map[i] = NULL;
		}
		
		/* White Player */
		_putPiece(0, 1, new Pawn(WHITE));
		_putPiece(1, 1, new Pawn(WHITE));
		_putPiece(2, 1, new Pawn(WHITE));
		_putPiece(3, 1, new Pawn(WHITE));
		_putPiece(4, 1, new Pawn(WHITE));
		_putPiece(5, 1, new Pawn(WHITE));
		_putPiece(6, 1, new Pawn(WHITE));
		_putPiece(7, 1, new Pawn(WHITE));
		_putPiece(0, 0, new Rook(WHITE));
		_putPiece(1, 0, new Knight(WHITE));
		_putPiece(2, 0, new Bishop(WHITE));
		_putPiece(4, 0, new King(WHITE));
		_putPiece(3, 0, new Queen(WHITE));
		_putPiece(5, 0, new Bishop(WHITE));
		_putPiece(6, 0, new Knight(WHITE));
		_putPiece(7, 0, new Rook(WHITE));
		
		
		/* Black Player */
		_putPiece(0, 6, new Pawn(BLACK));
		_putPiece(1, 6, new Pawn(BLACK));
		_putPiece(2, 6, new Pawn(BLACK));
		_putPiece(3, 6, new Pawn(BLACK));
		_putPiece(4, 6, new Pawn(BLACK));
		_putPiece(5, 6, new Pawn(BLACK));
		_putPiece(6, 6, new Pawn(BLACK));
		_putPiece(7, 6, new Pawn(BLACK));
		_putPiece(0, 7, new Rook(BLACK));
		_putPiece(1, 7, new Knight(BLACK));
		_putPiece(2, 7, new Bishop(BLACK));
		_putPiece(3, 7, new Queen(BLACK));
		_putPiece(4, 7, new King(BLACK));
		_putPiece(5, 7, new Bishop(BLACK));
		_putPiece(6, 7, new Knight(BLACK));
		_putPiece(7, 7, new Rook(BLACK));
}

//
// Board d'stor
//
Board::~Board()
{
	// delete move list contents
    list<Move *>::iterator it;
    for(it = gameMoveList.begin(); it != gameMoveList.end(); it++)
	{
		delete (*it);
    }

	// delete pieces
    for(int i=0; i<64; i++) {
		if( _map[i] != NULL)
			delete _map[i];
    }
}

//
// Add a piece to the pieceList
//
void Board::pieceListAdd(Piece *p)
{
    ADDPIECE(p);
    if( p->getKind() == PIECE_KING ) {
      if( p->getColour() == WHITE) {
	_whiteKing = (King *)p;
      }
      else {
	_blackKing = (King *)p;
      }
    }
}

//
// Delete a piece from the piece list
//
void Board::pieceListDel(Piece *p)
{
    CANCPIECE(p);
    if( p->getKind() == PIECE_KING ) {
      if( p->getColour() == WHITE ) {
	_whiteKing = NULL;
      } else {
	_blackKing = NULL;
      }
    }
}

//
// Add a piece on the board, only for first time!
//
void Board::_putPiece(const coord_t x, const coord_t y, Piece* p)
{
    // inseriamo il pezzo
    piece_at(x,y) = p;
    p->moveTo( xy2pos(x,y) );
    ADDPIECE(p);
	
    // se e' un re, va abbinato alla corrispettiva variabile
    if( p->getKind() == PIECE_KING ) {
		if( p->getColour() == WHITE )
			_whiteKing = (King *)p;
		else
			_blackKing = (King *)p;
    }
}

//
// Get a piece from specified position
//
Piece * Board::getPiece(const coord_t x, const coord_t y)
{
    if( COORDSOK(x, y) ) {
		return piece_at(x,y);
    } 
    else {
		return NULL;
    }
}

//
// Get a piece from specified position
//
Piece * Board::getPiece(const coord_t idx)
{
    return piece_dr(idx);
}

//
// Set a piece at specified position
//
void Board::setPiece(const coord_t pos, Piece *p)
{
    piece_dr(pos) = p;
}  

//
// Gimme a player
//  
PlayerIF * Board::getPlayer(colour_t colour)
{
    return ( colour == WHITE ? _whitePlayer : _blackPlayer );
}

//
// Gimme the King
//
Piece * Board::getKing(const colour_t colour)
{
    return ( colour == WHITE ? _whiteKing : _blackKing );
}

//
// Piece list
//
list<Piece *> * Board::listPieces(const colour_t colour)
{
    return (colour == WHITE ? &whiteList : &blackList);
}

//
// Rolls back last move, and deletes it
//
bool Board::moveRollback()
{
	Move *lastMove = NULL;
	try {
	  lastMove = (*gameMoveList.end());
	  gameMoveList.pop_back();
	  turn = ENEMY(turn);
	}
	catch(...)
	{
		return false;
	}
	return true;
}

//
// Confirms a move
//
void Board::moveFinalize(Move *move)
{
	gameMoveList.push_back(move);
}

//
// Tells who is winning
//
int Board::whoWins()
{
	if( isInCheck(WHITE)>0 && !canDefendCheck(WHITE) ) return BLACK;
	if( isInCheck(BLACK)>0 && !canDefendCheck(BLACK) ) return WHITE;

	return 0;
}

//
// Tells whether the game is finished or not.
// The game is NOT finished when no one is winning, and the game is
// not yet valued as tie.
//
bool Board::isGameFinished()
{
	bool stopFewPieces = false;
	
	// stop per poco materiale (solo i due re)
	list<Piece *> * lpWhite = listPieces(WHITE), * lpBlack = listPieces(BLACK);
	if( lpWhite->size() == 1 && lpBlack->size() == 1 )
	{
		stopFewPieces = true;
	}
	
	// stop per matto
 	pulchess_debug( "Is in check (WHITE): "      << isInCheck(WHITE) );
	pulchess_debug( "Is in check (BLACK): "      << isInCheck(BLACK) );
	pulchess_debug( "Can defend check (WHITE): " << canDefendCheck(WHITE) ); 	
	pulchess_debug( "Can defend check (BLACK): " << canDefendCheck(BLACK) );
	
    return( isInCheck(WHITE)>0 && !canDefendCheck(WHITE) ||
 			isInCheck(BLACK)>0 && !canDefendCheck(BLACK) ||
			stopFewPieces );
}

//
// Is the specified King under check?
//
int Board::isInCheck(const colour_t colour)
{
	return canEatThis(getKing(colour)->getPos(), colour, false);
}

//
// Can anyone eat the piace at "pos"?
//
bool Board::canEatThis(coord_t pos, const colour_t colour)
{	
	return ( canEatThis(pos, colour, false) > 0 );
}

//
// Can anyone eat the piace at "pos"? How many pieces can eat it?
//
int Board::canEatThis(coord_t pos, const colour_t colour, bool countTimes)
{
    list<Piece *> * lp = listPieces(ENEMY(colour));
    list<Piece *>::iterator lpIter;
	int count=0;
	
    for(lpIter = lp->begin(); lpIter != lp->end(); lpIter++)
	{
		if( (*lpIter)->isValidMove(pos, this) )
		{
			if(!countTimes) return 1;
			count++;
		}
    }
	
    return count;
}

//
// Can the player of "colour" defend from check?
//
bool Board::canDefendCheck(const colour_t colour)
{
    Move * m = checkDefenseMove(colour);
    if( m != NULL) {
		delete m;
		return true;
    }
    return false;
}

//
// Give a move for player "colour" in order to avoid check
//
Move * Board::checkDefenseMove(const colour_t colour)
{
    list<Piece *> * lp = listPieces(colour);
    list<Piece *>::iterator lpIter;
    list<Move *> mList;
    list<Move *>::iterator lmit;
	
    colour_t backupTurn = turn;
    turn = colour;
    
    // get moves for all pieces
    for(lpIter = lp->begin(); lpIter != lp->end(); lpIter++) {
		Piece * p = (*lpIter);
		p->listMoves(this, &mList);
    }
	
	// is any of these the right one?
    for(lmit = mList.begin(); lmit != mList.end(); lmit++) {
		try {
			(*lmit)->play(this);
			if( isInCheck(colour) == 0 ) {
				Move *savingMove;				
				(*lmit)->rewind(this);
				turn = backupTurn;
				savingMove = (*lmit)->copy();
				moveListDestroy(&mList);
				return savingMove;
			}
			(*lmit)->rewind(this);
		}
		catch(InvalidMoveException *ex)
		{
			pulchess_error( "Ho generato una mossa sbagliata... errore fatale!" );
			pulchess_error( ex->getMsg() );
			pulchess_error( "mossa: " << (*lmit)->toString() );
			exit(1);
		}
    }
    
    turn = backupTurn;
	moveListDestroy(&mList);
    return NULL;
}

//
// Evaluates board status
//
int Board::evaluate(colour_t colour)
{
    int val = 0;

    for(int i=0; i<64; i++) {
  		if(_map[i] != NULL) {			
  			val += _map[i]->getRank()          * _map[i]->getColour();
  			val += _map[i]->getPosEvaluation() * _map[i]->getColour();
  		}
    }
	
    return val;
}

//
// Total move count
//
int Board::getMoveCount()
{
    return moveCount;
}


//
// *** Start of BoardValue class implementation ***
//

//
// Class ctor
//
BoardValue::BoardValue(Board *b, coord_t depth, unsigned int dstTableSize)
{
	int base = 127;
    this->depth = depth;
    
    hashkey = 0;
    for(int i=0; i<64; i++) {
		if( b->getPiece(i) != NULL )
			map[i] = b->getPiece(i)->getValue();
		else
			map[i] = 0;
		
		hashkey = (base * hashkey + map[i]) % dstTableSize;
    }
}

//
// "deserialize" a BoardValue
//
BoardValue::BoardValue(const coord_t * rec, unsigned int dstTableSize)
{
	int base = 127;
	
	for(int i=0; i<64; i++) {
		map[i] = rec[i];
		hashkey = (base * hashkey + map[i]) % dstTableSize;
	}
}

//
// D'stor
//
BoardValue::~BoardValue()
{
}

//
// Equal operator
//
bool BoardValue::operator== (BoardValue &a)
{
	return (getHashKey() == a.getHashKey());
}

//
// Hashkey for this boardvalue
//
unsigned int BoardValue::getHashKey()
{
    return hashkey;
}

//
// Maximum depth this boardvalue is valid for
//
coord_t BoardValue::getDepth()
{
    return depth;
}

//
// Get piece map
//
coord_t * BoardValue::getMap()
{
    return map;
}

//
// Is this boardvalue usable for the specified one?
//
bool BoardValue::usableFor(BoardValue *b)
{
    for(int i=0; i<64; i++) {
		if( map[i] != b->getMap()[i] ) return false;
    }
	
    if( getDepth() >= b->getDepth() )
		return true;
	
    return false;
}

}; // end ns logic
}; // end ns pulchess