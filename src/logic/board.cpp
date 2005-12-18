/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Board and BoardValue implementation
 *
 * Created on 15-lug-2005
 * $Id$
 */
#include "logic/stdheader.h"

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
		for(byte i=0; i<64; i++) {
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
			
#ifdef DEBUG
			checkBoard();
#endif
		}
		catch(CoordsException e) {
			cerr << "Aggiunto pezzo in posizione errata!" << endl;
			exit(1);
		}
}

Board::~Board()
{
    // distruggi tutti i pezzi!
    for(byte i=0; i<64; i++) {
		if( _map[i] != NULL)
			delete _map[i];
    }
}


#ifdef DEBUG
void Board::printBoard()
{
    Piece * p;
    cout << "" << endl;
    for(int y=7; y>=0; y--) {
		for(int x=0; x<8; x++) {
			p = getPiece(x,y);
			if( p == NULL ) {
				cout << "-";
			}
			else {
				cout << p->getKindChr();
			}
			cout << " ";
		}
		cout << endl;
    }
    cout << "" << endl;
}

void Board::checkBoard()
{
    list<Piece *> * lP = listPieces(WHITE), * lPb = listPieces(BLACK);
    list<Piece *>::iterator lPiter;
    Piece * p;
    bool error = false;
	
    for(lPiter = lP->begin(); lPiter != lP->end(); lPiter++) {
		p = piece_at( (*lPiter)->getX(), (*lPiter)->getY());
		if( (*lPiter) != p ) {
			cerr << "Errore: scacchiera inconsistente!" << endl;
			error = true;
		}
    }
	
    for(lPiter = lPb->begin(); lPiter != lPb->end(); lPiter++) {
		p = piece_at( (*lPiter)->getX(), (*lPiter)->getY());
		if( (*lPiter) != p ) {
			cerr << "Errore: scacchiera inconsistente!" << endl;
			error = true;
		}
    }
	
    if( !error ) {
		cerr << "Scacchiera ok, pezzi ok." << endl;
    } else {
		exit(1);
    }
}
#endif

void Board::_checkCoords(const coord_t x, const coord_t y)
{
    if( !COORDSOK(x,y) )
		throw new CoordsException();
}

void Board::pieceListAdd(Piece *p)
{
    ADDPIECE(p);
}

void Board::pieceListDel(Piece *p)
{
    CANCPIECE(p);
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
Piece * Board::getPiece(const byte idx)
{
    return piece_dr(idx);
}


// Preleva il pezzo alla posizione specificata
//
// costo: O(1)
/////////////////////////////////////  
void Board::setPiece(const byte pos, Piece *p)
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
      _blackPlayer = tmpPlayer;
      tmpPlayer = NULL;
    }
    else {
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
	bool whiteInCheck = false;
	bool blackInCheck = false;
	
	whiteInCheck = isInCheck(WHITE);
	if( !whiteInCheck )
		blackInCheck = isInCheck(BLACK);
	
    whiteWins = blackInCheck && !canDefendCheck(BLACK);
    if( !whiteWins )
		blackWins = whiteInCheck && !canDefendCheck(WHITE);
    
    if( whiteWins )		return WHITE_WINS;
    if( blackWins )		return BLACK_WINS;
	if( whiteInCheck )	return WHITE_INCHECK;
	if( blackInCheck )	return BLACK_INCHECK;
    
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


// Verifica se il re del colore specificato e' in scacco
//
// costo: O(M * 64) --> O(M)     [M = numero di pedine del nemico]
//////////////////////////////////////////////////////////////////
bool Board::isInCheck(const colour_t colour)
{
	return canEatThis(getKing(colour)->getPos(), colour);
}


// Verifica se il pezzo alla posizione "pos" puo' essere mangiato
// dal nemico, specificato dal colore nel parametro "colour".
//
//////////////////////////////////////////////////////////////////
bool Board::canEatThis(coord_t pos, const colour_t colour)
{
    list<Piece *> * lp = listPieces(ENEMY(colour));
    list<Piece *>::iterator lpIter;
	
    // verifichiamo se qualcuno dei pezzi nemici puo'
	// mangiare, mettere in pericolo questo pezzo.
    for(lpIter = lp->begin(); lpIter != lp->end(); lpIter++) {
		if( (*lpIter)->isValidMove(pos, this) ) {
			Piece * p = (*lpIter);
			return true;
		}
    }
	
    // .. altrimenti niet!
    return false;
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
    
    // prova, per tutti i nostri pezzi
    for(lpIter = lp->begin(); lpIter != lp->end(); lpIter++) {
		Piece * p = (*lpIter);
		list<Move *> * lMoves = p->listMoves(this);
		mList.splice(mList.end(), *lMoves);
		moveListDestroy(lMoves);
		delete lMoves;
    }
	
    // a vedere tra tutte le mosse fattibili, se qualcuna ci toglie
    // dallo scacco. Se si, togliamo gli effetti temporanei della mossa e
    // ritorniamo vero.
    for(lmit = mList.begin(); lmit != mList.end(); lmit++) {
		try {
			(*lmit)->play(this);
			if( !isInCheck(colour) ) {
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
#ifdef DEBUG
			printBoard();
#endif
			exit(1);
		}
    }
    
    turn = backupTurn;
	moveListDestroy(&mList);
    return NULL;
}


// Valuta la "qualita'" di una certa configurazione
//
///////////////////////////////////////////////////
int Board::evaluate()
{
    int val = 0;
	int gameStat = 0;
    
    for(byte i=0; i<64; i++) {
		if(_map[i] != NULL) {			
			val += _map[i]->getRank()          * _map[i]->getColour();
			val += _map[i]->getPosEvaluation() * _map[i]->getColour();   
		}
    }
	
    gameStat = whoWins();
    
	// se e' una vittoria netta, il punteggio
	// e' assegnato direttamente.
    if( gameStat == WHITE_WINS ||
		gameStat == BLACK_WINS ) {
		val = gameStat;
    }
	// altrimenti se rappresenta un vantaggio,
	// come ad esempio il re nemico in scacco,
	// somma semplicemente il valore.
    else {
		val += gameStat;
    }
	
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
BoardValue::BoardValue(Board *b, byte depth, unsigned int dstTableSize)
{
    this->depth = depth;
    
    hashkey = 0;
    for(byte i=0; i<64; i++) {
		if( b->getPiece(i) != NULL )
			map[i] = b->getPiece(i)->getValue();
		else
			map[i] = 0;
		
		hashkey += (map[i] * i) % 97;
    }
	
    hashkey = hashkey % dstTableSize;
}

BoardValue::~BoardValue()
{
}

unsigned int BoardValue::getHashKey()
{
    return hashkey;
}

byte BoardValue::getDepth()
{
    return depth;
}

byte * BoardValue::getMap()
{
    return map;
}

bool BoardValue::usableFor(BoardValue *b)
{
    for(byte i=0; i<64; i++) {
		if( map[i] != b->getMap()[i] ) return false;
    }
	
    if( getDepth() >= b->getDepth() )
		return true;
	
    return false;
}

};
};