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
   assert_true( !facade->isGameFinished() );
   delete facade;

   // carico gioco errato, deve bloccarsi
   facade = new Pulchess(HUM_VS_HUM);
   assert_true( !facade->loadGame("./tests/games/igame000.game") );
   assert_true( !facade->isGameFinished() );
   delete facade;

   // carica gioco vero (Immortal Game)
   facade = new Pulchess(HUM_VS_HUM);
   assert_true( facade->loadGame("./tests/games/game001-immortal.game") );
   assert_true( facade->isGameFinished() );
   delete facade;

   // carica gioco prova enpassant
   // TODO: da spostare nel codice di test per il soldato
   facade = new Pulchess(HUM_VS_HUM);
   assert_true( facade->loadGame("./tests/games/game003-enpassant.game") );
   assert_true( !facade->isGameFinished() );
   delete facade;
}

void testSuitePulchessPulchess() {
	PULCHESS_CALLCASE(testLoadGame, "pulchess::loadGame");
}