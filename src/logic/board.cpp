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
		for(int i=0; i<64; i++) {
			_map[i] = NULL;
		}
		
		try {
			
			/* White Player */
			_putPiece(0, 1, new Soldier(WHITE));
			_putPiece(1, 1, new Soldier(WHITE));
			_putPiece(2, 1, new Soldier(WHITE));
			_putPiece(3, 1, new Soldier(WHITE));
			_putPiece(4, 1, new Soldier(WHITE));
			_putPiece(5, 1, new Soldier(WHITE));
			_putPiece(6, 1, new Soldier(WHITE));
			_putPiece(7, 1, new Soldier(WHITE));
			_putPiece(0, 0, new Tower(WHITE));
			_putPiece(1, 0, new Knight(WHITE));
			_putPiece(2, 0, new Bishop(WHITE));
			_putPiece(4, 0, new King(WHITE));
			_putPiece(3, 0, new Queen(WHITE));
			_putPiece(5, 0, new Bishop(WHITE));
			_putPiece(6, 0, new Knight(WHITE));
			_putPiece(7, 0, new Tower(WHITE));
			
			
			/* Black Player */
			_putPiece(0, 6, new Soldier(BLACK));
			_putPiece(1, 6, new Soldier(BLACK));
			_putPiece(2, 6, new Soldier(BLACK));
			_putPiece(3, 6, new Soldier(BLACK));
			_putPiece(4, 6, new Soldier(BLACK));
			_putPiece(5, 6, new Soldier(BLACK));
			_putPiece(6, 6, new Soldier(BLACK));
			_putPiece(7, 6, new Soldier(BLACK));
			_putPiece(0, 7, new Tower(BLACK));
			_putPiece(1, 7, new Knight(BLACK));
			_putPiece(2, 7, new Bishop(BLACK));
			_putPiece(3, 7, new Queen(BLACK));
			_putPiece(4, 7, new King(BLACK));
			_putPiece(5, 7, new Bishop(BLACK));
			_putPiece(6, 7, new Knight(BLACK));
			_putPiece(7, 7, new Tower(BLACK));
		}
		catch(CoordsException e) {
			cerr << "Aggiunto pezzo in posizione errata!" << endl;
			exit(1);
		}
}

Board::~Board()
{
    // distruggi tutti i pezzi!
    for(int i=0; i<64; i++) {
		if( _map[i] != NULL)
			delete _map[i];
    }
}

void Board::_checkCoords(const coord_t x, const coord_t y)
{
    if( !COORDSOK(x,y) )
		throw new CoordsException();
}

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

// used to add a piece on the board, only for first time!
void Board::_putPiece(const coord_t x, const coord_t y, Piece* p)
{
    _checkCoords(x, y);
    
    // inseriamo il pezzo
    piece_at(x,y) = p;
    p->moveTo( xy2pos(x,y) );
    //p->getColour() == WHITE ? whiteList.push_back(p) : blackList.push_back(p);
    ADDPIECE(p);
	
    // se e' un re, va abbinato alla corrispettiva variabile
    if( p->getKind() == PIECE_KING ) {
		if( p->getColour() == WHITE )
			_whiteKing = (King *)p;
		else
			_blackKing = (King *)p;
    }
}


// Preleva il pezzo alla posizione specificata
//
// costo: O(1)
/////////////////////////////////////
Piece * Board::getPiece(const coord_t x, const coord_t y)
{
    if( COORDSOK(x, y) ) {
		return piece_at(x,y);
    } 
    else {
		return NULL;
    }
}


// Preleva il pezzo alla posizione specificata
//
// costo: O(1)
/////////////////////////////////////
Piece * Board::getPiece(const coord_t idx)
{
    return piece_dr(idx);
}


// Preleva il pezzo alla posizione specificata
//
// costo: O(1)
/////////////////////////////////////  
void Board::setPiece(const coord_t pos, Piece *p)
{
    piece_dr(pos) = p;
}  


// Restituisce il giocatore del colore richiesto
//
// costo: O(1)
/////////////////////////////////////  
PlayerIF * Board::getPlayer(colour_t colour)
{
    return ( colour == WHITE ? _whitePlayer : _blackPlayer );
}


// Preleva il re di uno specifico colore
// 
// costo : O(1)
///////////////////////////////////////////////
Piece * Board::getKing(const colour_t colour)
{
    return ( colour == WHITE ? _whiteKing : _blackKing );
}


// Elenca i pezzi di uno specifico colore
//
// costo : O(1)
///////////////////////////////////////////////
list<Piece *> * Board::listPieces(const colour_t colour)
{
    return (colour == WHITE ? &whiteList : &blackList);
}


void Board::switchAutoThinking(const colour_t colour)
{
  static PlayerIF * tmpPlayer = NULL;

  // inizio auto-think
  if( tmpPlayer == NULL ) {
    if( colour == WHITE ) {
      tmpPlayer    = _blackPlayer;
      _blackPlayer = new CPUPlayer(BLACK);
    }
    else {
      tmpPlayer = _whitePlayer;
      _whitePlayer = new CPUPlayer(WHITE);
    }
  }

  // stop auto-think
  else {
    if( colour == WHITE ) {
	  delete _blackPlayer;
      _blackPlayer = tmpPlayer;
      tmpPlayer = NULL;
    }
    else {
	  delete _whitePlayer;
      _whitePlayer = tmpPlayer;
      tmpPlayer = NULL;
    }    
  }

}


// Dice chi sta vincendo la partita...
//
//////////////////////////////////////
int Board::whoWins()
{
    bool whiteWins = false;
    bool blackWins = false;
	int whiteInCheck = 0;
	int blackInCheck = 0;
	
	whiteInCheck = isInCheck(WHITE);
	if( whiteInCheck > 0)
		blackInCheck = isInCheck(BLACK);
	
    whiteWins = blackInCheck && !canDefendCheck(BLACK);
    if( whiteWins > 0 )
		blackWins = whiteInCheck && !canDefendCheck(WHITE);
    
	//
	// Ritorna il valore di valutazione.
	// Se mettiamo sotto scacco l'avversario, consideriamo
	// anche con quanti pezzi viene fatto.
	//
    if( whiteWins )			return WHITE_WINS;
    if( blackWins )			return BLACK_WINS;
	if( whiteInCheck > 0 )	return ( WHITE_INCHECK + whiteInCheck*CHECK_PLUS );
	if( blackInCheck > 0)	return ( BLACK_INCHECK - blackInCheck*CHECK_PLUS );
    
    return 0;
}


// Dice se il gioco e' finito. Il gioco non e' finito quando nessuno dei due
// sta vincendo, e la partita non e' ancora considerata "patta".
//
////////////////////////////
bool Board::isGameFinished()
{
    colour_t gameStat = whoWins();
    if(gameStat != 0)
		return true;
    else
		return false;
}


// Verifica se il re del colore specificato e' in scacco.
// Ritorna il numero di pezzi che mettono in scacco.
//
// costo: O(M * 64) --> O(M)     [M = numero di pedine del nemico]
//////////////////////////////////////////////////////////////////
int Board::isInCheck(const colour_t colour)
{
	return canEatThis(getKing(colour)->getPos(), colour, true);
}

bool Board::canEatThis(coord_t pos, const colour_t colour)
{	
	return ( canEatThis(pos, colour, false) > 0 );
}

// Verifica se il pezzo alla posizione "pos" puo' essere mangiato
// dal nemico, specificato dal colore nel parametro "colour".
// se countTimes e' true, conta il numero di pezzi che possono mangiare.
//
//////////////////////////////////////////////////////////////////
int Board::canEatThis(coord_t pos, const colour_t colour, bool countTimes)
{
    list<Piece *> * lp = listPieces(ENEMY(colour));
    list<Piece *>::iterator lpIter;
	int count=0;
	
    // verifichiamo se qualcuno dei pezzi nemici puo'
	// mangiare, mettere in pericolo questo pezzo.
    for(lpIter = lp->begin(); lpIter != lp->end(); lpIter++) {
		if( (*lpIter)->isValidMove(pos, this) ) {
			if(!countTimes) return 1;
			count++;
		}
    }
	
    // ritorna il numero di pezzi che possono mangiare.
    return count;
}

bool Board::canDefendCheck(const colour_t colour)
{
    Move * m = checkDefenseMove(colour);
    if( m != NULL) {
		delete m;
		return true;
    }
    return false;
}


// E' possibile effettuare una mossa per evitare lo scacco?
//
///////////////////////////////////////////
Move * Board::checkDefenseMove(const colour_t colour)
{
    list<Piece *> * lp = listPieces(colour);
    list<Piece *>::iterator lpIter;
    list<Move *> mList;
    list<Move *>::iterator lmit;
	
    colour_t backupTurn = turn;
    turn = colour;
    
    // genera le mosse di tutti i pezzi
    for(lpIter = lp->begin(); lpIter != lp->end(); lpIter++) {
		Piece * p = (*lpIter);
		p->listMoves(this, &mList);
    }
	
    // a vedere tra tutte le mosse fattibili, se qualcuna ci toglie
    // dallo scacco. Se si, togliamo gli effetti temporanei della mossa e
    // ritorniamo vero.
    for(lmit = mList.begin(); lmit != mList.end(); lmit++) {
		try {
			(*lmit)->play(this);
			if( isInCheck(colour) > 0 ) {
				Move *savingMove;
				
				// -- riportiamo lo stato in uno coerente --
				(*lmit)->rewind(this);
				turn = backupTurn;
				savingMove = (*lmit)->copy();
				//
				moveListDestroy(&mList);
				return savingMove;
			}
			(*lmit)->rewind(this);
		}
		catch(InvalidMoveException *ex) {
			cerr << "Ho generato una mossa sbagliata... errore fatale!" << endl;
			cerr << ex->getMsg();
			printf("Pezzo da spostare: x %d   y %d   kind:%d\n",
				   (*lmit)->getSourceX(),
				   (*lmit)->getSourceY(),
				   getPiece((*lmit)->getSourceX(), (*lmit)->getSourceY())->getKind());
			cerr << endl;
			exit(1);
		}
    }
    
    turn = backupTurn;
	moveListDestroy(&mList);
    return NULL;
}


// Valuta la "qualita'" di una certa configurazione
//
// TODO: salvare l'ultimo pezzo che ha mangiato
//		 bonus se mangia pezzo poco importante
///////////////////////////////////////////////////
int Board::evaluate(colour_t colour)
{
    int val = 0;
    
    // valutazione del materiale
    for(int i=0; i<64; i++) {
  		if(_map[i] != NULL) {			
  			val += _map[i]->getRank()          * _map[i]->getColour();
  			val += _map[i]->getPosEvaluation() * _map[i]->getColour();   
  		}
    }
    
    //
    // controllo che la mossa non metta il re in scacco
    // TODO: questo controllo e' molto lento.
    //       bisogna trovare una strategia diversa per
    //       evitare quelle mosse che mettono il re in scacco
    //       basta evitare le mosse predette con profondita' 1!!!
    //
    //if( isInCheck(colour) )
    //{
    //    if( colour == WHITE ) return BLACK_WINS;
    //    if( colour == BLACK ) return WHITE_WINS;    
    //}
    //
	
    return val;
}


// Il numero di mosse totali effettuate
//
/////////////////////////
int Board::getMoveCount()
{
    return moveCount;
}


/////////////////////////////////////////////////////////////////////
//
// Start of BoardValue class implementation
//
/////////////////////////////////////////////////////////////////////
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


// "Deserializza" una boardvalue
BoardValue::BoardValue(const coord_t * rec, unsigned int dstTableSize)
{
	int base = 127;
	
	for(int i=0; i<64; i++) {
		map[i] = rec[i];
		hashkey = (base * hashkey + map[i]) % dstTableSize;
	}
}

BoardValue::~BoardValue()
{
}

bool BoardValue::operator== (BoardValue &a)
{
	return (getHashKey() == a.getHashKey());
}

unsigned int BoardValue::getHashKey()
{
    return hashkey;
}

coord_t BoardValue::getDepth()
{
    return depth;
}

coord_t * BoardValue::getMap()
{
    return map;
}

bool BoardValue::usableFor(BoardValue *b)
{
    for(int i=0; i<64; i++) {
		if( map[i] != b->getMap()[i] ) return false;
    }
	
    if( getDepth() >= b->getDepth() )
		return true;
	
    return false;
}

};
};
