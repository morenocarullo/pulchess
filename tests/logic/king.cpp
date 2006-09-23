#include "stdheader.h"
#include "pulchess.H"
#include "tests.h"
#include <iostream>

using namespace pulchess::logic;
using namespace std;

static char * suitename = "king suite";

static void testListMoves_start()
{
  Board b(new CPUPlayer(WHITE), new CPUPlayer(BLACK));
  King * k;
  
  Move	m1(xy2pos(4,3), xy2pos(4,1)),
		m2(xy2pos(4,5), xy2pos(4,7));
  m1.play(&b); // white's move
  m2.play(&b); // black's move

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

static void testCastlings()
{
   // carico gioco corretto, non deve dare errori	
   Pulchess * pulchess = new Pulchess(HUM_VS_HUM);
   assert_true( pulchess->loadGame("./tests/games/game004-castling-1.game") );
   delete pulchess;
}

void testSuiteKing() {
	PULCHESS_CALLCASE(testListMoves_start, "king::testListMoves_start");
	PULCHESS_CALLCASE(testCastlings, "king::testCastlings");
}