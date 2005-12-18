/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Hashcache implementation
 *
 * Created on 15-lug-2005
 * $Id$
 */
#include "logic/stdheader.h"
#include <stdlib.h>

namespace pulchess { namespace logic {
	
HashCache::HashCache(unsigned int hashsize)
{
	size = hashsize;
	_init();
}

HashCache::HashCache()
{
	size = 110023;
	_init();
}

HashCache::~HashCache()
{
}

void HashCache::_init()
{
#ifdef DEBUG
	printf("hashcache table size: %d\n", size);
#endif
	statsmissct = 0;
	statshitct  = 0;
	
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
	unsigned int key = t->getHashKey();
	
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

void HashCache::statsHit()
{
	statshitct++;
}

void HashCache::statsMiss()
{
	statsmissct++;
}

unsigned int
HashCache::getStatsHit()
{
	return statshitct;
}

unsigned int
HashCache::getStatsMiss()
{
	return statsmissct;
}

};
};