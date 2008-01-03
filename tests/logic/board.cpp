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
#include "book.H"
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

static void test_LoadFromFen_TurnFlag()
{
  Board b1( "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  Board b2( "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");

  assert_true( b1.turn == WHITE );
  assert_true( b2.turn == BLACK );
}

static void test_LoadFromFen_EnpassantFlag()
{
  Board board_with_ep( "8/7p/p5pb/4k3/P1pPn3/8/P5PP/1rB2RK1 b - d3 0 28" );
  Board board_wo_ep( "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  assert_true( board_with_ep.enpassant == xy2pos(3,2) );
  assert_true( board_wo_ep.enpassant == NO_POSITION );
}

static void test_Evaluate()
{
	Board board;
	
	// posizione iniziale = 0
	assert_true( board.Evaluate() == 0 );
}

static void test_IsInCheck_black()
{
  Board board( "rnbqkbnr/pp3ppp/2B1p3/3p4/8/4P2P/PPPP1PP1/RNBQK1NR b KQkq - 0 4" );
  pulchess_board     = &board;
  pulchess_the_white = new CPUPlayer(WHITE);
  pulchess_the_black = new CPUPlayer(BLACK);

  assert_true( board.IsInCheck(BLACK) > 0 );
  assert_true( board.CanDefendCheck(BLACK) );
  assert_true( board.IsInCheck(WHITE) == 0 );
}

static void test_IsInCheck_black_2() 
{
  Board board( "rnbqkbnr/pp3ppp/2B1p3/8/3p4/4P2P/PPPP1PP1/RNBQK1NR w KQkq - 0 5" );
  pulchess_board     = &board;
  pulchess_the_white = new CPUPlayer(WHITE);
  pulchess_the_black = new CPUPlayer(BLACK);

  assert_true( board.IsInCheck(BLACK) > 0 );
  assert_true( board.IsInCheck(WHITE) == 0 );
}

static void test_GenerateMoves_InCheck()
{
  Board board( "rnbqkbnr/pp3ppp/2B1p3/3p4/8/4P2P/PPPP1PP1/RNBQK1NR b KQkq - 0 4" );
  pulchess_board     = &board;
  pulchess_the_white = new CPUPlayer(WHITE);
  pulchess_the_black = new CPUPlayer(BLACK);

  vector<Move *> mList;
  vector<Move *>::iterator mList_iter;
 
  board.GenerateMoves(mList, true);

  /* 6 legal moves: b8c6, b8d7, c8d7, d8c7, e8e7, b7c6 */
  assert_true( mList.size() == 6 );
}

static void test_GenerateMoves_perft_strange_1()
{
  Board board( "r3k2r/p1ppqpb1/bn2pnp1/1B1PN3/1p2P3/2N2Q1p/PPPB1PPP/R3K2R b KQkq -" );
  pulchess_board     = &board;
  pulchess_the_white = new CPUPlayer(WHITE);
  pulchess_the_black = new CPUPlayer(BLACK);
  vector<Move *> mList;

  board.GenerateMoves(mList, true);

  /* 39 legal moves */
  assert_true( mList.size() == 39 );
}

static void test_GenerateMoves_perft_strange_2()
{
  Board board( "r3k2r/p1pNqpb1/bn2pnp1/3P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq -" );
  pulchess_board     = &board;
  pulchess_the_white = new CPUPlayer(WHITE);
  pulchess_the_black = new CPUPlayer(BLACK);
  vector<Move *> mList;

  board.GenerateMoves(mList, true);

  /* 45 legal moves */
  assert_true( mList.size() == 45 );
}

static void test_EnpassantFlag()
{
  Board board( "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -" );
  pulchess_board     = &board;
  pulchess_the_white = new HumanPlayer(WHITE);
  pulchess_the_black = new HumanPlayer(BLACK);

  assert_true( pulchess_the_white->DoMove("a2a4") );
  assert_true( pulchess_board->enpassant != NO_POSITION );
  assert_true( pulchess_board->enpassant == xy2pos(0,3) );
}

static void test_CastlingsFlag_Start()
{
  Board board;
  pulchess_board     = &board;
  pulchess_the_white = new HumanPlayer(WHITE);
  pulchess_the_black = new HumanPlayer(BLACK);

  assert_true( pulchess_board->castlingFlags ==
				CASTLING_WHITE_K | CASTLING_WHITE_Q | CASTLING_BLACK_K | CASTLING_BLACK_Q );
}

static void test_CastlingsFlag_AfterRookMove_1()
{
  Board board( "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -" );
  pulchess_board     = &board;
  pulchess_the_white = new HumanPlayer(WHITE);
  pulchess_the_black = new HumanPlayer(BLACK);

  assert_true( pulchess_board->castlingFlags ==
				CASTLING_WHITE_K | CASTLING_WHITE_Q | CASTLING_BLACK_K | CASTLING_BLACK_Q );
				
  // white moves, castling white queenside drops
  assert_true( pulchess_the_white->DoMove("a1b1") );
  assert_true( pulchess_board->castlingFlags ==
 				CASTLING_WHITE_K | CASTLING_BLACK_K | CASTLING_BLACK_Q );

  // black moves, castling black queenside drops
  assert_true( pulchess_the_black->DoMove("a8b8") );
  assert_true( pulchess_board->castlingFlags ==
 				CASTLING_WHITE_K | CASTLING_BLACK_K );
}

static void test_BoardValue_basic()
{
	Board b1( "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Board b2( "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
	
	BoardValue *bv1 = new BoardValue(&b1, 99, Book::bookSize);
	BoardValue *bv2 = new BoardValue(&b2, 99, Book::bookSize);
	
	assert_true(*bv1 != *bv2);
	
	delete bv1;
}

void testSuiteBoard()
{
	PULCHESS_CALLCASE(test_moveRollback_one,	"board::test_moveRollback_one");
    PULCHESS_CALLCASE(test_GetLastMove,			"board::test_GetLastMove");
    PULCHESS_CALLCASE(test_LoadFromFen,			"board::test_LoadFromFen");
    PULCHESS_CALLCASE(test_LoadFromFen_TurnFlag, "board::test_LoadFromFen_TurnFlag");
	PULCHESS_CALLCASE(test_LoadFromFen_EnpassantFlag, "board::test_LoadFromFen_EnpassantFlag");
	PULCHESS_CALLCASE(test_Evaluate,			"board::test_Evaluate");
	PULCHESS_CALLCASE(test_IsInCheck_black,    "board::test_IsInCheck_black");
	PULCHESS_CALLCASE(test_IsInCheck_black_2,    "board::test_IsInCheck_black_2");
	PULCHESS_CALLCASE(test_GenerateMoves_InCheck, "board::test_test_GenerateMoves_InCheck");
	PULCHESS_CALLCASE(test_GenerateMoves_perft_strange_1, "board::test_GenerateMoves_perft_strange_1");
	PULCHESS_CALLCASE(test_GenerateMoves_perft_strange_2, "board::test_GenerateMoves_perft_strange_2");
	PULCHESS_CALLCASE(test_EnpassantFlag, "board::test_EnpassantFlag");
	PULCHESS_CALLCASE(test_CastlingsFlag_Start, "board::test_CastlingsFlag_Start");
	PULCHESS_CALLCASE(test_CastlingsFlag_AfterRookMove_1, "board::test_CastlingsFlag_AfterRookMove_1");
	PULCHESS_CALLCASE(test_BoardValue_basic,    "boardvalue::test_basic");
}