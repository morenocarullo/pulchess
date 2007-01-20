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

static char * suitename = "board suite";

//
// Simple rollback test case
//
static void test_moveRollback_one()
{
  Board b(new HumanPlayer(WHITE), new HumanPlayer(BLACK));
  Move * m1;
  int moveCt1, moveCt2, moveCt3;

  try
  {
    // status quo ante
    BoardValue boardValueStart(&b, 99, 97);
    moveCt1 = b.GetMoveCount();

    // a1a3 move
    m1 = new Move( xy2pos(0,3), xy2pos(0,1), 0 );
    m1->Play(&b);
    b.MoveFinalize(m1);
    BoardValue boardValueMove(&b, 99, 97);
    moveCt2 = b.GetMoveCount();
    assert_true( boardValueStart != boardValueMove );
    assert_true( moveCt2 == moveCt1+1 );

    // now rollback and check
    assert_true( b.MoveRollback() == true );
    BoardValue boardValueRoll(&b, 99, 97);
    moveCt3 = b.GetMoveCount();
    assert_true( boardValueStart == boardValueRoll );
    assert_true( moveCt3 == moveCt1 );
  }
  catch(...)
  {
    assert_true( 0 == "exception thrown!" );
  }
}

static void test_GetLastMove()
{
  Board b(new HumanPlayer(WHITE), new HumanPlayer(BLACK));
  Move * m1;

  // status quo ante
  BoardValue boardValueStart(&b, 99, 97);

  // a1a3 move
  m1 = new Move( xy2pos(0,3), xy2pos(0,1), 0 );
  m1->Play(&b);
  b.MoveFinalize(m1);
  Move * lastMove = b.GetLastMove();

  // e' la stessa mossa che ho fatto?
  assert_true( m1->toString() == lastMove->toString() );
}

void testSuiteBoard() {
	PULCHESS_CALLCASE(test_moveRollback_one, "board::test_moveRollback_one");
    PULCHESS_CALLCASE(test_GetLastMove, "board::test_GetLastMove");
}