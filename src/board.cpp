/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Board and BoardValue implementation
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
#include "pulchess.H"

#define piece_at(X,Y)  _map[ (Y)*8 + (X) ]
#define piece_dr(I)    _map[ (I) ]

#define CANCPIECE(X) { (X)->colour == WHITE ? whiteList.remove(X) : blackList.remove(X); }
#define ADDPIECE(X) { (X)->colour == WHITE ? whiteList.push_back(X) : blackList.push_back(X); }
#define VIOLATURNO(X) ( (X)->colour != turn )

namespace pulchess { namespace logic {
	
//
// Global, singleton instance.
//
Board*	Board::board = NULL;

//
// Board ctor
//	
Board::Board()
{		
  LoadFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

//
// Board ctor - FEN version
//
// C++ does not support calling of ctor inside of ctor, so a LoadFromFen()
// member has been implemented.
//
Board::Board(string ebdString)
{
  LoadFromFen(ebdString);
}

//
// Load from FEN.
// See: http://www.very-best.de/pgn-spec.htm#16.a
//
void Board::LoadFromFen(string ebdString)
{
  int ct=0,row=7,col=0;
  char c;
  bool stop=false;
  colour_t colour;

  // nullset pieces
  for(int i=0; i<64; i++) {
    piece_dr(i) = NULL;
  }
  
  // reset en passant
  enpassant = NO_POSITION;

  // reset number of moves
  moveCount      = 0;
  fiftyMovesRule = 0;

  // start loading FEN position data
  while(!stop) {
    c=ebdString[ct];
    switch(c) {
      case 'p':
      case 'P':
        colour = (c == 'P') ? WHITE : BLACK; 
        FirstInsertOfPiece(col,row,new Pawn(colour)); col++;
        break;
      case 'r':
      case 'R':
        colour = (c == 'R') ? WHITE : BLACK; 
        FirstInsertOfPiece(col,row,new Rook(colour)); col++;
        break;
      case 'n':
      case 'N':
        colour = (c == 'N') ? WHITE : BLACK; 
        FirstInsertOfPiece(col,row,new Knight(colour)); col++;
        break;
      case 'b':
      case 'B':
        colour = (c == 'B') ? WHITE : BLACK; 
        FirstInsertOfPiece(col,row,new Bishop(colour)); col++;
        break;
      case 'q':
      case 'Q':
        colour = (c == 'Q') ? WHITE : BLACK; 
        FirstInsertOfPiece(col,row,new Queen(colour)); col++;
        break;
      case 'k':
      case 'K':
        colour = (c == 'K') ? WHITE : BLACK; 
        FirstInsertOfPiece(col,row,new King(colour)); col++;
        break;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
        col += (c-'0');
        break;
      case '/':
        row--; col=0;
        break;
      case ' ':
        stop=true;
        break;
    }
    ct++;
  }

  // turn
  turn = ebdString[ct++] == 'w' ? WHITE : BLACK;

  // castling

  // en passant

  // clock

  // 50-move stuff	
}

//
// Board d'stor
//
Board::~Board()
{
  // delete move list contents
  list<Move *>::iterator it;
  for(it = gameMoveList.begin(); it != gameMoveList.end(); it++) {
    delete (*it);
  }

  // delete pieces
  for(int i=0; i<64; i++) {
    if( piece_dr(i) != NULL)
      delete piece_dr(i);
  }
}

//
// Print board to standard output
//
void Board::Print()
{
  Piece *p = NULL;
  cout << "" << endl;
  for(int y=7; y>=0; y--) {
    printf("%d   ", y+1);
    for(int x=0; x<8; x++) {
      p = GetPiece(x,y);
      if( p == NULL ) {
        cout << "-";
      }
      else {
       if( p->colour == WHITE) {
         cout << (char)toupper(p->getKindChr());
       }
       else {
         cout << p->getKindChr();
       }
      }
      cout << " ";
    }
    cout << endl;
  }

  cout << endl << "    ";
  for(int x=0; x<8; x++) {
    printf("%c ", 'a'+x);
  }
  cout << "" << endl << endl;
}

//
// Add a piece to the pieceList
//
void Board::PieceListAdd(Piece *p)
{
  ADDPIECE(p);
  if( p->GetKind() == PIECE_KING ) {
    if( p->colour == WHITE) {
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
void Board::PieceListDel(Piece *p)
{
    CANCPIECE(p);
    if( p->GetKind() == PIECE_KING ) {
      if( p->colour == WHITE ) {
	_whiteKing = NULL;
      } else {
	_blackKing = NULL;
      }
    }
}

//
// Add a piece on the board, only for first time (board creation)
//
void inline Board::FirstInsertOfPiece(const coord_t x, const coord_t y, Piece* p)
{
    piece_at(x,y) = p;
    p->moveTo( xy2pos(x,y) );
    PieceListAdd(p);
}

//
// Get a piece from specified position
//
Piece * Board::GetPiece(const coord_t x, const coord_t y)
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
Piece * Board::GetPiece(const coord_t idx)
{
    return piece_dr(idx);
}

//
// Set a piece at specified position
//
void Board::SetPiece(const coord_t pos, Piece *p)
{
    piece_dr(pos) = p;
}  

//
// Gimme a player
//  
Player * Board::GetPlayer(colour_t colour)
{
    return ( colour == WHITE ? pulchess_the_white : pulchess_the_black );
}

//
// Gimme the King
//
Piece * Board::GetKing(const colour_t colour)
{
    return ( colour == WHITE ? _whiteKing : _blackKing );
}

//
// Piece list
//
list<Piece *> * Board::ListPieces(const colour_t colour)
{
    return (colour == WHITE ? &whiteList : &blackList);
}

//
// Rolls back last move, and deletes it
//
bool Board::MoveRollback()
{
	Move *lastMove = NULL;
	try {
	  lastMove = gameMoveList.back();
	  lastMove->Rewind();
	  gameMoveList.pop_back();
	  delete lastMove;
	  turn = ENEMY(turn);
	}
	catch(...)
	{
		return false;
	}
	return true;
}

//
// Get move list
//
list<Move *> *
Board::GetMoveList()
{
  return &gameMoveList;
}

//
// Get latest move
//
Move * Board::GetLastMove()
{
   return gameMoveList.back();
}

//
// Confirms a move, and push clocks.
//
void Board::MoveFinalize(Move *move)
{
  if( move != NULL ) {
	gameMoveList.push_back(move);

    string messaggio1("Finalized move: ");
    string messaggio = messaggio1 + move->toString();
    Pulchess::WriteLog(messaggio);
  }
  
  if( turn == WHITE ) { pulchess_the_black->StopClock(); pulchess_the_white->PushClock(); }
  else                { pulchess_the_white->StopClock(); pulchess_the_black->PushClock(); }
}

//
// Tells who is winning
//
int Board::WhoWins()
{
	if( IsInCheck(WHITE)>0 && !CanDefendCheck(WHITE) ) return BLACK;
	if( IsInCheck(BLACK)>0 && !CanDefendCheck(BLACK) ) return WHITE;

	return 0;
}

//
// Tells whether the game is finished or not.
// The game is NOT finished when no one is winning, and the game is
// not yet valued as tie.
//
bool Board::IsGameFinished()
{
  // few pieces (two kings only)
  list<Piece *> * lpWhite = ListPieces(WHITE), * lpBlack = ListPieces(BLACK);
  if( lpWhite->size() == 1 && lpBlack->size() == 1 ) {
    return true;
  }
	
  // checkmate!
  if( IsInCheck(WHITE)>0 && !CanDefendCheck(WHITE) ||
      IsInCheck(BLACK)>0 && !CanDefendCheck(BLACK) ) {
	pulchess_debug("Checkmate!");
    return true;
  }

  // 50 moves draw rule!
  if( fiftyMovesRule >= 50) {
    pulchess_debug("Draw by 50 moves rule.");
    return true;
  }

  return false;
}

//
// Is the specified King under check?
//
int Board::IsInCheck(const colour_t colour)
{
	return CanEatThis(GetKing(colour)->getPos(), colour, false);
}

//
// Can anyone eat the piace at "pos"?
//
bool Board::CanEatThis(coord_t pos, const colour_t colour)
{	
	return ( CanEatThis(pos, colour, false) > 0 );
}

//
// Can anyone eat the piace at "pos"? How many pieces can eat it?
//
int Board::CanEatThis(coord_t pos, const colour_t colour, bool countTimes)
{
    list<Piece *> * lp = ListPieces(ENEMY(colour));
    list<Piece *>::iterator lpIter;
	int count=0;
	
    for(lpIter = lp->begin(); lpIter != lp->end(); lpIter++)
	{
		if( (*lpIter)->IsValidMove(pos) )
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
bool Board::CanDefendCheck(const colour_t colour)
{
    Move * m = CheckDefenseMove(colour);
    if( m != NULL) {
		delete m;
		return true;
    }
    return false;
}

//
// Give a move for player "colour" in order to avoid check
//
Move * Board::CheckDefenseMove(const colour_t colour)
{
    list<Piece *> * lp = ListPieces(colour);
    list<Piece *>::iterator lpIter;
    vector<Move *> mList;
    vector<Move *>::iterator lmit;
	
    colour_t backupTurn = turn;
    turn = colour;
    
    // get moves for all pieces
    for(lpIter = lp->begin(); lpIter != lp->end(); lpIter++) {
		Piece * p = (*lpIter);
		p->listMoves(&mList);
    }
	
	// is any of these the right one?
    for(lmit = mList.begin(); lmit != mList.end(); lmit++) {
		try {
			(*lmit)->Play();
			if( IsInCheck(colour) == 0 ) {
				Move *savingMove;				
				(*lmit)->Rewind();
				turn = backupTurn;
				savingMove = (*lmit)->copy();
				moveListDestroy(&mList);
				return savingMove;
			}
			(*lmit)->Rewind();
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
int Board::Evaluate(colour_t colour)
{
    int val = 0, tmppos = 0;
    coord_t wkingpos, bkingpos;
    
    // material
    for(int i=0; i<64; i++) {
  		if(piece_dr(i) != NULL) {			
  			val += piece_dr(i)->GetRank()          * piece_dr(i)->colour;
  			val += piece_dr(i)->getPosEvaluation() * piece_dr(i)->colour;
  		}
    }
    
    // white king protection
    if( _whiteKing != NULL)
    {
    	wkingpos = _whiteKing->getPos();
    	tmppos = wkingpos+7;
    	if( OKCOORDS(tmppos) && _map[tmppos] != NULL && _map[tmppos]->colour == WHITE) val += 1 * WHITE;	
    	tmppos = wkingpos+8;
    	if( OKCOORDS(tmppos) && _map[tmppos] != NULL && _map[tmppos]->colour == WHITE) val += 1 * WHITE;
    	tmppos = wkingpos+9;
    	if( OKCOORDS(tmppos) && _map[tmppos] != NULL && _map[tmppos]->colour == WHITE) val += 1 * WHITE;
    	tmppos = wkingpos-7;
    	if( OKCOORDS(tmppos) && _map[tmppos] != NULL && _map[tmppos]->colour == WHITE) val += 1 * WHITE;	
    	tmppos = wkingpos-8;
    	if( OKCOORDS(tmppos) && _map[tmppos] != NULL && _map[tmppos]->colour == WHITE) val += 1 * WHITE;
    	tmppos = wkingpos-9;
    	if( OKCOORDS(tmppos) && _map[tmppos] != NULL && _map[tmppos]->colour == WHITE) val += 1 * WHITE;	    	  
    }

    // black king protection
    if(_blackKing != NULL )
    {
    	bkingpos = _blackKing->getPos();	
    	tmppos = bkingpos+7;
    	if( OKCOORDS(tmppos) && _map[tmppos] != NULL && _map[tmppos]->colour == BLACK) val += 1 * BLACK;	
    	tmppos = bkingpos+8;
    	if( OKCOORDS(tmppos) && _map[tmppos] != NULL && _map[tmppos]->colour == BLACK) val += 1 * BLACK;
    	tmppos = bkingpos+9;
    	if( OKCOORDS(tmppos) && _map[tmppos] != NULL && _map[tmppos]->colour == BLACK) val += 1 * BLACK;
    	tmppos = bkingpos-7;
    	if( OKCOORDS(tmppos) && _map[tmppos] != NULL && _map[tmppos]->colour == BLACK) val += 1 * BLACK;	
    	tmppos = bkingpos-8;
    	if( OKCOORDS(tmppos) && _map[tmppos] != NULL && _map[tmppos]->colour == BLACK) val += 1 * BLACK;
    	tmppos = bkingpos-9;
    	if( OKCOORDS(tmppos) && _map[tmppos] != NULL && _map[tmppos]->colour == BLACK) val += 1 * BLACK;
    }
    
	
    return val;
}

//
// Total move count
//
int Board::GetMoveCount()
{
    return moveCount;
}


//
// *** Start of BoardValue class implementation ***
//

//
// Class ctor
//
BoardValue::BoardValue(coord_t depth, unsigned int dstTableSize)
{
	int base = 127;
    this->depth = depth;
    
    hashkey = 0;
    for(int i=0; i<64; i++) {
		if( pulchess_board->GetPiece(i) != NULL )
			map[i] = pulchess_board->GetPiece(i)->GetValue();
		else
			map[i] = 0;
		
		hashkey = (base * hashkey + map[i]) % dstTableSize;
    }
}

//
// "Deserialize" a BoardValue
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
	return (GetHashKey() == a.GetHashKey());
}

//
// Not equal operator
//
bool BoardValue::operator!= (BoardValue &a)
{
	return (GetHashKey() != a.GetHashKey());
}

//
// Hashkey for this boardvalue
//
unsigned int BoardValue::GetHashKey()
{
    return hashkey;
}

//
// Maximum depth this boardvalue is valid for
//
coord_t BoardValue::GetDepth()
{
    return depth;
}

//
// Get piece map
//
coord_t * BoardValue::GetMap()
{
    return map;
}

//
// Is this boardvalue usable for the specified one?
//
bool BoardValue::UsableFor(BoardValue *b)
{
    for(int i=0; i<64; i++) {
		if( map[i] != b->GetMap()[i] ) return false;
    }
	
    if( GetDepth() >= b->GetDepth() )
		return true;
	
    return false;
}

}; // end ns logic
}; // end ns pulchess
