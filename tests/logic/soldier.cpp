/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Pawn unit test
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

static char * suitename = "soldier suite";

static void testListMoves_start()
{
  Board b(new CPUPlayer(WHITE), new CPUPlayer(BLACK));
  Pawn * s;

  s = (Pawn *)b.GetPiece(0,1);

  list<Move *> * mList = new list<Move *>();
  list<Move *>::iterator lmit;
  s->listMoves(&b, mList);

  for(lmit = mList->begin();  lmit != mList->end();  lmit++){
	BoardValue bvalue(&b, 99, 97);
    (*lmit)->play( &b );
    (*lmit)->rewind( &b );
	BoardValue postBvalue(&b, 99, 97);
	assert_true( bvalue == postBvalue );
  }
}

void testSuitePawn() {
	PULCHESS_CALLCASE(testListMoves_start, "soldier::testListMoves_start");
}