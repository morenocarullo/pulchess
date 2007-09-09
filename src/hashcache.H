/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Hashcache interface
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
#ifndef _pulchess_util_hashcache_
#define _pulchess_util_hashcache_

#include "stdheader.h"

namespace pulchess { namespace logic {

  //! A class for transposition tables
  class HashCache
  {

  private:
    int * _map;
    BoardValue ** _vals;
    unsigned int  size;
    void _Init();

  public:
    HashCache(unsigned int size);
    HashCache();
    ~HashCache();

    void Insert(BoardValue *t, int value);
	void Clear();
    BoardValue * Get(unsigned int key);
    int GetValue(unsigned int key);
    unsigned int getSize();
  };

};
};

#endif
