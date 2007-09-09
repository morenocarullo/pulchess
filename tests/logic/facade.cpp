/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo moreno.carullo [at] pulc [dot] it
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Pulchess main facade unit test
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
#include "pulchess.H"
#include "tests.h"
#include <iostream>

using namespace pulchess::logic;
using namespace std;

static char * suitename = "pulchess facade";
static void testLoadGame()
{	
   // carico gioco corretto, non deve dare errori	
   Pulchess * facade = new Pulchess(HUM_VS_HUM);
   assert_true( facade->loadGame("./tests/games/game000.game") );
   assert_true( !facade->IsGameFinished() );
   delete facade;

   // carico gioco errato, deve bloccarsi
   facade = new Pulchess(HUM_VS_HUM);
   assert_true( !facade->loadGame("./tests/games/igame000.game") );
   assert_true( !facade->IsGameFinished() );
   delete facade;

   // carica gioco vero (Immortal Game)
   facade = new Pulchess(HUM_VS_HUM);
   assert_true( facade->loadGame("./tests/games/game001-immortal.game") );
   assert_true( facade->IsGameFinished() );
   delete facade;

   // carica gioco vero (Moreno vs Pulchess - 21/9/2006)
   facade = new Pulchess(HUM_VS_HUM);
   assert_true( facade->loadGame("./tests/games/game002-morec.game") );
   assert_true( facade->IsGameFinished() );
   delete facade;

   // carica gioco prova enpassant
   // TODO: da spostare nel codice di test per il soldato
   facade = new Pulchess(HUM_VS_HUM);
   // TODO assert_true( facade->loadGame("./tests/games/game003-enpassant.game") ); --> DA RIVEDERE SECONDO LE REGOLE!!!
   //assert_true( !facade->IsGameFinished() );
   delete facade;
}

static void testResetGame()
{
   Pulchess *pulchess = new Pulchess(HUM_VS_HUM);
   pulchess->Init();

   assert_true( pulchess != NULL );
   assert_true( pulchess->IsHuman(PULCHESS_WHITE) );
   assert_true( pulchess->IsHuman(PULCHESS_BLACK) );

   pulchess->ResetMode(HUM_VS_CPU);
   assert_true( pulchess->IsHuman(PULCHESS_WHITE) );
   assert_true( !pulchess->IsHuman(PULCHESS_BLACK) );

   pulchess->ResetMode(CPU_VS_CPU);
   assert_true( !pulchess->IsHuman(PULCHESS_WHITE) );
   assert_true( !pulchess->IsHuman(PULCHESS_BLACK) );

   pulchess->ResetMode(CPU_VS_HUM);
   assert_true( !pulchess->IsHuman(PULCHESS_WHITE) );
   assert_true( pulchess->IsHuman(PULCHESS_BLACK) );

   delete pulchess;
}

void testSuitePulchessPulchess() {
	PULCHESS_CALLCASE(testLoadGame, "pulchess::loadGame()");
	PULCHESS_CALLCASE(testResetGame, "pulchess::ResetGame()");
}