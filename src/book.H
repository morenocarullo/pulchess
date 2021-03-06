/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Book interface
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
#ifndef _pulchess_logic_book_h_
#define _pulchess_logic_book_h_

#include "board.H"

namespace pulchess { namespace logic {

typedef struct {
	Move *m;
	BoardValue *b;
} BookMove;

class Book {	

	public:
		static int bookSize;
		static list<BookMove *> *map;
	
		//! Load a book file
		static void Init(); 
		static bool Load();
		static bool Load(const char *filename);
		static void Insert(BoardValue *b, Move *m);
		static void Insert(string fenBoard, string moveCmd);
		static Move * Search(BoardValue *b);
};

}}
#endif