/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo moreno.carullo [at] pulc [dot] it
 * LICENSE: GPL, see license.txt in project root
 * FILE:    King piece unit test
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
 * Created on ?
 * $Id$
 */
#include "stdheader.h"
#include "pulchess.H"
#include "tests.h"
#include <iostream>

using namespace pulchess::logic;
using namespace std;

static char * suitename = "king suite";

static void testCastlings()
{
   // carico gioco corretto, non deve dare errori	
   Pulchess * pulchess = new Pulchess(HUM_VS_HUM);
   assert_true( pulchess->loadGame("./tests/games/game004-castling-1.game") );
   delete pulchess;

   // castling lato nero
   Board b("rnbqk2r/pppppppp/8/8/8/8/PPPPP3/RNBQKBNR b KQ - 0 1");
   pulchess_board     = &b;
   pulchess_the_white = new HumanPlayer(WHITE);
   pulchess_the_black = new HumanPlayer(BLACK);
   assert_true( pulchess_the_black->DoMove("e8g8") );
   delete pulchess_the_white;
   delete pulchess_the_black;
}

void testSuiteKing() {
	PULCHESS_CALLCASE(testCastlings, "king::testCastlings");
}