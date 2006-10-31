/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Hashcache implementation
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
#include <stdlib.h>

namespace pulchess { namespace logic {
	
HashCache::HashCache(unsigned int hashsize)
{
	size = hashsize;
	_init();
}

HashCache::HashCache()
{
	size = 97;
	_init();
}

HashCache::~HashCache()
{
	for(unsigned int i=0; i<size; i++) {
		if( _vals[i] != NULL )
			delete _vals[i];
	}
}

void HashCache::_init()
{
	_map  = new int[size]; 
	_vals = new BoardValue*[size];
	
	for(unsigned int i=0; i<size; i++) {
		_vals[i] = NULL;
		_map[i] = 0;
	}  
}

unsigned int HashCache::getSize()
{
	return size;
}

void HashCache::insert(BoardValue *t, int val)
{
	unsigned int key = t->GetHashKey();
	
	BoardValue *bv   = _vals[ key ];
	
	_vals[ key ] = t;
	_map[ key ] = val;
	
	if( bv != NULL )
		delete bv;
}

BoardValue * HashCache::get(unsigned int key)
{
	return _vals[ key ];
}

int HashCache::getValue(unsigned int key)
{
	return _map[ key ];
}

};
};