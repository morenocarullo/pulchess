#include "stdheader.h"
#include "tests.h"
#include <iostream>

using namespace pulchess::logic;
using namespace std;

static char * suitename = "rook suite";

static void testListMoves_start()
{
  Board b(new CPUPlayer(WHITE,6,1,false), new CPUPlayer(BLACK,6,1,false));
  Rook * s;

  s = (Rook *)b.getPiece(0,1);

  list<Move *> * mList = new list<Move *>();
  list<Move *>::iterator lmit;
  s->listMoves(&b, mList);

  for(lmit = mList->begin();  lmit != mList->end();  lmit++){
	BoardValue bvalue(&b, 99, 97);
    (*lmit)->play( &b );
    (*lmit)->rewind( &b );
	BoardValue postBvalue(&b, 99, 97);
	assert_true( bvalue == postBvalue );
  }
}

void testSuiteRook() {
	PULCHESS_CALLCASE(testListMoves_start, "rook::testListMoves_start");
}
