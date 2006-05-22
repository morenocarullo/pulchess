/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE:    Pulchess Logic Facade interface
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
#ifndef _pulchess_logic_facade_h_
#define _pulchess_logic_facade_h_

#include <string>
#include "playerif.H"

#define CPU_VS_CPU 0
#define HUM_VS_CPU 1
#define HUM_VS_HUM 2

#define PULCHESS_WHITE -1
#define PULCHESS_BLACK 1

typedef char gamemode_t;
typedef struct {
  char kind;
  char colour;
} cellinfo_t;

using namespace std;

namespace pulchess { namespace logic {

  // human controller facade
  /////////////////////////////
  class HumanControllerFacade
  {
  public:
	virtual ~HumanControllerFacade() {};
    virtual string getMove() = 0;
    virtual char getSoldierPiece() = 0;
  };


  // main Pulchess Facade Class
  /////////////////////////////
  class Facade
  {

  private:
    void * board;
    int    turn;
    gamemode_t gameMode;
    HumanControllerFacade * whiteController;
    HumanControllerFacade * blackController;
    PlayerIF * whitePlayer;
    PlayerIF * blackPlayer;

  public:

    Facade(gamemode_t gameMode);
    ~Facade();

    // imposta i controller
    void setController(HumanControllerFacade * c, int colour);
    
    // inizializza il gioco
    void init();

    // chiude il gioco
    void shutdown();
    
    // ricostruisce la scacchiera
    cellinfo_t getCellInfo(int x, int y);

    // il gioco e' finito?
    bool isGameFinished();

    // a chi tocca?
    int whoPlaysNow();
	
	// il prossimo giocatore e' umano?
	bool isHuman();

    // gioca la mossa del turno attuale
    bool requestPlay();
	
    // chi vince?
    int gameInfo();
  };

};
};

#endif
