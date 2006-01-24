/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Hashcache interface
 *
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
 *
 * Created on 15-lug-2005
 * $Id$
 */
#ifndef _pulchess_util_hash_
#define _pulchess_util_hash_

#include "logic/stdheader.h"

namespace pulchess { namespace logic {

  class HashCache {

  private:
    int * _map;
    BoardValue ** _vals;
    unsigned int  size;
    unsigned int statshitct;
    unsigned int statsmissct;
    void _init();

  public:
    HashCache(unsigned int size);
    HashCache();
    ~HashCache();

    void insert(BoardValue *t, int value);
    BoardValue * get(unsigned int key);
    int getValue(unsigned int key);
    unsigned int getSize();
    void statsHit();
    void statsMiss();
    unsigned int getStatsHit();
    unsigned int getStatsMiss();
  };

};
};

#endif
