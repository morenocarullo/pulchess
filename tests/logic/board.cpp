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
  Board b;
  Move * m1;
  int moveCt1, moveCt2, moveCt3;

  // board & players setup
  pulchess_board     = &b;
  pulchess_the_white = new CPUPlayer(WHITE);
  pulchess_the_black = new CPUPlayer(BLACK);

  try
  {
    // status quo ante
    BoardValue boardValueStart(99, 97);
    moveCt1 = b.GetMoveCount();

    // a1a3 move
    m1 = new Move( xy2pos(0,3), xy2pos(0,1), 0 );
    m1->Play();
    b.MoveFinalize(m1);
    BoardValue boardValueMove(99, 97);
    moveCt2 = b.GetMoveCount();
    assert_true( boardValueStart != boardValueMove );
    assert_true( moveCt2 == moveCt1+1 );

    // now rollback and check
    assert_true( b.MoveRollback() == true );
    BoardValue boardValueRoll(99, 97);
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
  Board b;
  Move * m1;

  // board & players setup
  pulchess_board     = &b;
  pulchess_the_white = new CPUPlayer(WHITE);
  pulchess_the_black = new CPUPlayer(BLACK);

  // status quo ante
  BoardValue boardValueStart(99, 97);

  // a1a3 move
  m1 = new Move( xy2pos(0,3), xy2pos(0,1), 0 );
  m1->Play();
  b.MoveFinalize(m1);
  Move * lastMove = b.GetLastMove();

  // e' la stessa mossa che ho fatto?
  assert_true( m1->toString() == lastMove->toString() );
}

static void test_LoadFromFen()
{
	Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	
	Piece *whiteRookSx = board.GetPiece(0,0);
	assert_true( whiteRookSx->GetKind() == PIECE_ROOK );
	assert_true( whiteRookSx->colour == WHITE );
}

// TODO: move in CPUPlayer tests
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

// TODO: move in CPUPlayer tests
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


static void test_Evaluate()
{
	Board board;
	
	// posizione iniziale = 0
	assert_true( board.Evaluate(WHITE) == 0 );
	assert_true( board.Evaluate(BLACK) == 0 );
}

void testSuiteBoard()
{
	PULCHESS_CALLCASE(test_moveRollback_one,	"board::test_moveRollback_one");
    PULCHESS_CALLCASE(test_GetLastMove,			"board::test_GetLastMove");
    PULCHESS_CALLCASE(test_LoadFromFen,			"board::test_LoadFromFen");
	PULCHESS_CALLCASE(test_Evaluate,			"board::test_Evaluate");
	PULCHESS_CALLCASE(test_KingSafety_as_white, "board::test_KingSafety1");
	PULCHESS_CALLCASE(test_KingSafety_as_black,	"board::test_KingSafety2");
}