/*
* PROJECT: PulCHESS, a Computer Chess program
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

namespace pulchess { namespace logic {

int					Book::bookSize = 1197;
list<BookMove *> *	Book::map = NULL;
bool				Book::usable = false;

// Load an book file
bool Book::load(const char *filename)
{
	int i;
	map = new list<BookMove *>[bookSize];
	usable = true;
	return true;
}

// Insert a new move
void Book::insert(BoardValue *b, Move *m)
{
	if( !usable ) return;

	BookMove * bm = new BookMove;
	bm->m = m;
	bm->b = b;
	map[b->getHashKey()].push_front( bm );
}

// Search for this move
Move * Book::search(BoardValue *b)
{
	if( !usable ) return NULL;
	
	list<BookMove *> * lst = &map[b->getHashKey()];
	list<BookMove *>::iterator it;
	
	for(it=lst->begin(); it!=lst->end(); it++) {
		if( (*it)->b->usableFor(b) ) {
			return (*it)->m;
		}
	}
	
	return NULL;
}

}}