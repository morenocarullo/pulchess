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

static void testListMoves_start()
{
  Board b(new CPUPlayer(WHITE), new CPUPlayer(BLACK));
  King * k;
  
  Move	m1(xy2pos(4,3), xy2pos(4,1)),
		m2(xy2pos(4,5), xy2pos(4,7));
  m1.Play(&b); // white's move
  m2.Play(&b); // black's move

  k = (King *)b.GetPiece(4,0);

  list<Move *> * mList = new list<Move *>();
  list<Move *>::iterator lmit;
  k->listMoves(&b, mList);

  for(lmit = mList->begin();  lmit != mList->end();  lmit++){
	BoardValue bvalue(&b, 99, 97);
    (*lmit)->Play( &b );
    (*lmit)->Rewind( &b );
	BoardValue postBvalue(&b, 99, 97);
	assert_true( bvalue == postBvalue );
  }
}

static void testCastlings()
{
   // carico gioco corretto, non deve dare errori	
   Pulchess * pulchess = new Pulchess(HUM_VS_HUM);
   assert_true( pulchess->loadGame("./tests/games/game004-castling-1.game") );
   delete pulchess;
}

void testSuiteKing() {
	PULCHESS_CALLCASE(testListMoves_start, "king::testListMoves_start");
	PULCHESS_CALLCASE(testCastlings, "king::testCastlings");
}