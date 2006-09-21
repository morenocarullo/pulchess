#include "stdheader.h"
#include "tests.h"
#include <iostream>

using namespace pulchess::logic;
using namespace std;

static char * suitename = "soldier suite";

static void testListMoves_start()
{
  Board b(new CPUPlayer(WHITE), new CPUPlayer(BLACK));
  Pawn * s;

  s = (Pawn *)b.getPiece(0,1);

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

void testSuitePawn() {
	PULCHESS_CALLCASE(testListMoves_start, "soldier::testListMoves_start");
}