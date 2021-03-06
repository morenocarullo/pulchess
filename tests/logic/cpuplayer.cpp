/*
  PulCHESS, a Computer Chess program
                by Moreno Carullo
 
  About this file:
         Unit tests for CPUPlayer, aka the main A.I. container.
 
  License:
         GPL v2, see license.txt in project root.
 
  Version:
         $Id$

*/
#include "stdheader.h"
#include "tests.h"
#include <iostream>

using namespace pulchess::logic;
using namespace std;

static char * suitename = "cpuplayer suite";

static void test_KingSafety_as_black()
{
	Board b("5bnr/2Q1kp1p/1Npppp2/8/2Pr4/P7/1P1N1PPP/R1B1K2R b KQ - 0 20");
	
    // board & players setup
    pulchess_board     = &b;
    pulchess_the_white = new CPUPlayer(WHITE);
    pulchess_the_black = new CPUPlayer(BLACK);

	// this is in check
    assert_true( b.IsInCheck(BLACK) );

	// play a move, remove from check
    pulchess_the_black->DoMove("");
    assert_true( !b.IsInCheck(BLACK) );

	delete pulchess_the_white;
	delete pulchess_the_black;
}

static void test_KingSafety_as_white()
{
	Board b("rn1kqbnr/pp2pppp/2pp4/8/8/1b1pp3/P4PPP/RNBKQBNR w KQ - 0 20");
	
    // board & players setup
    pulchess_board     = &b;
    pulchess_the_white = new CPUPlayer(WHITE);
    pulchess_the_black = new CPUPlayer(BLACK);

	// this is in check
    assert_true( b.IsInCheck(WHITE) );

	// play a move, remove from check
    pulchess_the_white->DoMove("");
    assert_true( !b.IsInCheck(WHITE) );

	delete pulchess_the_white;
	delete pulchess_the_black;
}

void testSuiteCpuPlayer()
{
	PULCHESS_CALLCASE(test_KingSafety_as_white, "board::test_KingSafety1");
	PULCHESS_CALLCASE(test_KingSafety_as_black,	"board::test_KingSafety2");
}