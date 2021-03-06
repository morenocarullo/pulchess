/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo moreno.carullo [at] pulc [dot] it
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Human player unit test
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

static char * suitename = "humanplayer suite";

//! Test correctness of pawn promotion
static void testGetPawnPromotion()
{
   Pulchess * facade = new Pulchess(HUM_VS_HUM);
   assert_true( facade->LoadGame("./tests/games/game005-pawnprom.game") );
   assert_true( !facade->IsGameFinished() );
   
   cellinfo_t cell = facade->getCellInfo(0, 7);
   assert_true( cell.kind  == 'n');
   assert_true( cell.colour == 'w');
   delete facade;
}

void testSuiteHumanPlayer() {
	PULCHESS_CALLCASE(testGetPawnPromotion, "humanplayer::testGetPawnPromotion");
}