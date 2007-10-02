/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Book implementation
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
 * Created on 12-feb-2006
 * $Id$
 */
#include "book.H"
#include <stdio.h>

namespace pulchess { namespace logic {

int					Book::bookSize = 97;
list<BookMove *> *	Book::map = NULL;

void Book::Init()
{
  if(map == NULL) {
	map = new list<BookMove *>[bookSize];
  }
}

// Load default books
bool Book::Load()
{
	Init();
	
	//
	// ----- Load Defaults mini-book moves -----
	// e2e4
	Insert( "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "e2e4" );
	Insert( "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1", "e7e5" );
	
	// ----- End Loading Defaults mini-book moves! -----
	//
	
	return Load("data/book");
}

//
// Load a book file
//
bool Book::Load(const char *filename)
{
  /*
	TODO: load from EBD or from COMPILED book!
  */
  return false;
}

//
// Insert a new move
//
void Book::Insert(BoardValue *b, Move *m)
{
	BookMove * bm = new BookMove;
	bm->m = m;
	bm->b = b;
	map[b->GetHashKey()].push_front( bm );
	
	pulchess_debug("Inserted move " << m->toString() << " in book. Hashkey is: " << b->GetHashKey());
}

//
// Insert a move, given the FEN position and the simple algebrical notation.
//
void Book::Insert(string fenBoard, string moveCmd)
{
	Board _board(fenBoard);
    Move *move = Move::GetMoveFactory(moveCmd, _board.turn);
	Insert( new BoardValue(&_board, 99, bookSize), move );
}

//
// Search for this move
//
Move * Book::Search(BoardValue *b)
{
	list<BookMove *> * lst = &map[b->GetHashKey()];
	list<BookMove *>::iterator it;
	
	for(it=lst->begin(); it!=lst->end(); it++) {
		pulchess_debug("Verifying move: " << (*it)->m->toString());
		if( (*it)->b->UsableFor(b) ) {
			return (*it)->m;
		}
	}
	
	pulchess_debug("Unsuccessful search in book with key: " << b->GetHashKey());
	
	return NULL;
}

}}