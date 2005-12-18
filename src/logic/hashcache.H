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
