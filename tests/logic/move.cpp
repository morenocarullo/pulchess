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
#include <algorithm>

using namespace pulchess::logic;
using namespace std;

static char * suitename = "move suite";


//
// Play a move, and check for "assertions"
//
static void test_move_one()
{
	// SEE r.58 for tests, after Board singleton refactoring
}


//
// Test sui contatori della regola delle 50 mosse.
//
static void test_50moves_params()
{
	Board b;
	pulchess_board = &b;

	//
	// 	controllo lo stato della fiftyMovesRule.
	//
	assert_true(b.fiftyMovesRule == 0);
	
	//
	// applico una mossa a caso non di pedone -> fiftyMovesRule + 1
	// rollback -> controllo che sia uguale a prima
	//
	Move m1( xy2pos(0,2), xy2pos(1,0), 0); // cavallo: b1 -> a3
	m1.Play();
	assert_true(b.fiftyMovesRule == 1);
	m1.Rewind();
	assert_true(b.fiftyMovesRule == 0);
	
	//
	// TODO: caricare un caso in cui si puo' mangiare,
	//       e quindi testare che 50movesRule viene impostato a 0.
	//
}


//
// Test move ordering
//
static void test_move_ordering()
{
	vector<Move> moves;
	
	Move m1( xy2pos(4,3), xy2pos(4,1), 100 );
	Move m2( xy2pos(5,4), xy2pos(5,6), 4 );
	Move m3( xy2pos(5,4), xy2pos(4,3), 0 );
	
	moves.push_back(m1);
	moves.push_back(m2);
	moves.push_back(m3);
	
	sort(moves.begin(), moves.end());
	
	assert_true( moves[0].rating == 100 );
	assert_true( moves[1].rating == 4 );
	assert_true( moves[2].rating == 0 );
}

void testSuiteMove() {
	PULCHESS_CALLCASE(test_move_one, "move::test_move_one");
	PULCHESS_CALLCASE(test_50moves_params, "move::test_50moves_params");
	PULCHESS_CALLCASE(test_move_ordering, "move::test_move_ordering");
}