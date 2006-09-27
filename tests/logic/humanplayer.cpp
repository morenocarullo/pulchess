#include "stdheader.h"
#include "pulchess.H"
#include "tests.h"
#include <iostream>

using namespace pulchess::logic;
using namespace std;

static char * suitename = "humanplayer suite";

//! Test correctness of pawn promotion
static void testGetPawnPromotion()
{
   Pulchess * facade = new Pulchess(HUM_VS_HUM);
   assert_true( facade->loadGame("./tests/games/game005-pawnprom.game") );
   assert_true( !facade->isGameFinished() );
   
   cellinfo_t cell = facade->getCellInfo(0, 7);
   assert_true( cell.kind  == 'n');
   assert_true( cell.colour == 'w');
   delete facade;
}

void testSuiteHumanPlayer() {
	PULCHESS_CALLCASE(testGetPawnPromotion, "humanplayer::testGetPawnPromotion");
}