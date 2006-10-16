/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE:    Pulchess Logic facade interface
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
* Created on 15-lug-2005
* $Id$
*/
#ifndef _pulchess_logic_pulchess_h_
#define _pulchess_logic_pulchess_h_

#include <string>
#include "playerif.H"

const unsigned char CPU_VS_CPU = 0;
const unsigned char HUM_VS_CPU = 1;
const unsigned char CPU_VS_HUM = 2;
const unsigned char HUM_VS_HUM = 3;

const int PULCHESS_WHITE = 1;
const int PULCHESS_BLACK = -1;

const int PULCHESS_STATUS_ZERO = 0;
const int PULCHESS_STATUS_INIT = 1;
const int PULCHESS_STATUS_END  = 2;

typedef char gamemode_t;
typedef struct {
  char kind;
  char colour;
} cellinfo_t;

using namespace std;

namespace pulchess { namespace logic {

  class Pulchess
  {

  private:
    void * board;
    int    turn;
	int engineStatus;
    gamemode_t gameMode;
    PlayerIF * whitePlayer;
    PlayerIF * blackPlayer;

  public:

    Pulchess(gamemode_t gameMode);
    ~Pulchess();
    
    // inizializza il gioco
    void init();

    // chiude il gioco
    void shutdown();
    
    // ricostruisce la scacchiera
    cellinfo_t getCellInfo(int x, int y);

	// stampa su output
	void printBoard();

    // il gioco e' finito?
    bool isGameFinished();

    // a chi tocca?
    int whoPlaysNow();
	
	// il prossimo giocatore e' umano?
	bool isHuman();

    // gioca la mossa del turno attuale
    bool gameCommand(string cmd);
	
    // chi vince?
    int gameInfo();

	// carica un gioco
	bool loadGame(const char *gamePath);
  };

};
};

#endif
