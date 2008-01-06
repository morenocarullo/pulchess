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
   assert_true( facade->LoadGame("./tests/games/game000.game") );
   assert_true( !facade->IsGameFinished() );
   delete facade;

   // carico gioco errato, deve bloccarsi
   facade = new Pulchess(HUM_VS_HUM);
   assert_true( !facade->LoadGame("./tests/games/igame000.game") );
   assert_true( !facade->IsGameFinished() );
   delete facade;

   // carica gioco vero (Immortal Game)
   facade = new Pulchess(HUM_VS_HUM);
   assert_true( facade->LoadGame("./tests/games/game001-immortal.game") );
   assert_true( facade->IsGameFinished() );
   delete facade;

   // carica gioco vero (Moreno vs Pulchess - 21/9/2006)
   facade = new Pulchess(HUM_VS_HUM);
   assert_true( facade->LoadGame("./tests/games/game002-morec.game") );
   assert_true( facade->IsGameFinished() );
   delete facade;

   // carica gioco vero (Pulchess vs Spike - 27/10/2007)   (ex problema Queenside Castling)
   facade = new Pulchess(HUM_VS_HUM);
   assert_true( facade->LoadGame("./tests/games/pulc-spike.game") );
   delete facade;

   // carica gioco vero (Pulchess vs Spike, 2 - 27/10/2007) (problema Queenside Castling)
   facade = new Pulchess(HUM_VS_HUM);
   assert_true( facade->LoadGame("./tests/games/pulc-spike2.game") );
   delete facade;

   // carica gioco prova enpassant
   // TODO: da spostare nel codice di test per il soldato
   //facade = new Pulchess(HUM_VS_HUM);
   // TODO assert_true( facade->LoadGame("./tests/games/game003-enpassant.game") ); --> DA RIVEDERE SECONDO LE REGOLE!!!
   //assert_true( !facade->IsGameFinished() );
   //delete facade;
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

static void testPerft_1()
{
	Pulchess *pulchess = new Pulchess(HUM_VS_HUM);
	pulchess->Init( "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -" );
	assert_true( pulchess->Perft(1) == 48 );
	assert_true( pulchess->Perft(2) == 2039 );
	// FAILS :(  assert_true( pulchess->Perft(3) == 97862 );
}

static void testPerft_start()
{
	Pulchess *pulchess = new Pulchess(HUM_VS_HUM);
	pulchess->Init();
	assert_true( pulchess->Perft(3) == 8902 );
	assert_true( pulchess->Perft(4) == 197281 );
	assert_true( pulchess->Perft(5) == 4865609 );
}


void testSuitePulchessPulchess() {
	PULCHESS_CALLCASE(testLoadGame, "pulchess::LoadGame()");
	PULCHESS_CALLCASE(testResetGame, "pulchess::ResetGame()");
	PULCHESS_CALLCASE(testPerft_1, "pulchess::testPerft_1()");
	PULCHESS_CALLCASE(testPerft_start, "pulchess::testPerft_start()");
}