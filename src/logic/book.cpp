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
#include <stdio.h>

namespace pulchess { namespace logic {

int					Book::bookSize = 97;
list<BookMove *> *	Book::map = NULL;
bool				Book::usable = false;

// Load default books
bool Book::load()
{
	return load("data/book");
}

// Load a book file
bool Book::load(const char *filename)
{
	FILE *fp;
	int ct=0;
	coord_t buff[67];
	BoardValue *bv;
	Move *move;
	
	map = new list<BookMove *>[bookSize];
	fp  = fopen(filename, "rb");
	if( fp == NULL ) {
		return false;
	}
	while(!feof(fp)) {
		if( fread(buff, sizeof(coord_t), 67, fp) < 67 ) {
			return false;
		}
		bv   = new BoardValue(buff, Book::bookSize);
		move = new Move(*(buff+65), *(buff+66));
		//printf("%d %d\n", move->getSourceX(), move->getSourceY());
		insert(bv, move);
		ct++;
	}
	
	fclose(fp);
	
	if( ct>0 ) {		
		usable = true;
		return true;
	}
	else {
		return false;
	}
}

bool Book::save(const char *filename)
{
	FILE *fp;
	int i;
	coord_t buff[2];
	list<BookMove *> * lst;
	list<BookMove *>::iterator it;
	
	fp = fopen(filename, "wb+");
	
	if( fp == NULL ) {
		return false;
	}
	
	for(i=0; i<bookSize; i++) {
		lst = &map[i];
		for(it=lst->begin(); it!=lst->end(); it++) {
			// scrive la boardvalue
			buff[0] = 0;
			fwrite((*it)->b->getMap(), sizeof(coord_t), 64, fp);
			fwrite(buff, sizeof(coord_t), 1, fp);	// TODO: castling state + turn state
			
			// scrive la mossa			
			buff[0] = (*it)->m->getSrcIdx();
			buff[1] = (*it)->m->getDstIdx();
			fwrite(buff, sizeof(coord_t), 2, fp);
		}
	}
	
	fclose(fp);
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