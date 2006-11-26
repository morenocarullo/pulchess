/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Board unit test
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
#include "tests.h"
#include <iostream>

using namespace pulchess::logic;
using namespace std;

static char * suitename = "move suite";

//
// Play a move, and check for "assertions"
//
static void test_move_one()
{
  Board b(new HumanPlayer(WHITE), new HumanPlayer(BLACK));
  Move *m1, *m2, *m3;

  try
  {
    // e2e4
    m1 = new Move( xy2pos(4,3), xy2pos(4,1) );
    assert_true( m1->Play(&b) == 0 );
 
    // f7f5
	m2 = new Move( xy2pos(5,4), xy2pos(5,6) );
	assert_true( m2->Play(&b) == 0 );
	
	// e4f5
	m3 = new Move( xy2pos(5,4), xy2pos(4,3) );
    assert_true( m3->Play(&b) == PIECE_RANK_PAWN );
  }
  catch(...)
  {
    assert_true( 0 == "exception thrown!" );
  }
}

void testSuiteMove() {
	PULCHESS_CALLCASE(test_move_one, "move::test_move_one");
}