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
    Board * board;
	int engineStatus;
    gamemode_t gameMode;
    Player * whitePlayer;
    Player * blackPlayer;

  public:

    Pulchess(gamemode_t gameMode);
    ~Pulchess();

    //! Get a string containing engine's name and version
    static string GetPulchessVersion();

    //! Init game
    void Init();

    //! Reset game
    void ResetMode(gamemode_t gameMode);

    //! Set timecontrol
    void SetTimecontrol(int movesToPlay, int secondsForMoves);

    //! Close engine
    void Shutdown();
    
    // ricostruisce la scacchiera
    cellinfo_t getCellInfo(int x, int y);

	// stampa su output
	void printBoard();

    // il gioco e' finito?
    bool IsGameFinished();

    // a chi tocca?
    int whoPlaysNow();
	
	// il prossimo giocatore e' umano?
	bool IsHuman();
	
	//! Is the requested player human?
	bool IsHuman(const colour_t colour);

    // gioca la mossa del turno attuale
    bool gameCommand(string &cmd);

    //! Try to play an empty command
    bool gameCommand();

    // e' una mossa?
    bool IsMove(string &cmd);

    //! Ritorna l'ultima mossa
    Move * GetLastMove();

    //! Returns a string containing game history report
    string GetGameMovesReport();
	
    // chi vince?
    int gameInfo();

	// carica un gioco
	bool loadGame(const char *gamePath);
  };

}; // end logic namespace
}; // end pulchess namespace

#endif