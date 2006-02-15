#include "stdheader.h"
#include "tests.h"
#include <iostream>

using namespace pulchess::logic;
using namespace std;

static char * suitename = "king suite";

static void testListMoves_start()
{
  Board b(new CPUPlayer(WHITE,6,1,false), new CPUPlayer(BLACK,6,1,false));
  King * k;
  
  Move m(xy2pos(4,3), xy2pos(4,1));
  m.play(&b);

  k = (King *)b.getPiece(4,0);

  list<Move *> * mList = new list<Move *>();
  list<Move *>::iterator lmit;
  k->listMoves(&b, mList);

  for(lmit = mList->begin();  lmit != mList->end();  lmit++){
	BoardValue bvalue(&b, 99, 97);
    (*lmit)->play( &b );
    (*lmit)->rewind( &b );
	BoardValue postBvalue(&b, 99, 97);
	assert_true( bvalue == postBvalue );
  }
}

void testSuiteKing() {
	PULCHESS_CALLCASE(testListMoves_start, "king::testListMoves_start");
}

