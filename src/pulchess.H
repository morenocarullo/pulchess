/*
  PulCHESS, a Computer Chess program
                by Moreno Carullo
 
  About this file:
         This file contains the main pulchess facade (interface),  
         used to expose to other programs the pulchess logic.
 
  License:
         GPL v2, see license.txt in project root.
 
  Version:
         $Id$

*/
#ifndef _pulchess_logic_pulchess_h_
#define _pulchess_logic_pulchess_h_

#include <iostream>
#include <fstream>
#include <string>
#include "player.H"

const unsigned char CPU_VS_CPU = 0;
const unsigned char HUM_VS_CPU = 1;
const unsigned char CPU_VS_HUM = 2;
const unsigned char HUM_VS_HUM = 3;

const int PULCHESS_WHITE = 1;
const int PULCHESS_BLACK = -1;

const int PULCHESS_STATUS_ZERO  = 0;
const int PULCHESS_STATUS_INIT  = 1;
const int PULCHESS_STATUS_START = 2;
const int PULCHESS_STATUS_END   = 4;

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
	int engineStatus;
    gamemode_t gameMode;
    static ofstream log;

  public:

    Pulchess(gamemode_t gameMode);
    ~Pulchess();

    //! Get a string containing engine's name and version
    static string GetPulchessVersion();

    //! Init game
    void Init();
    
    //! Init game with a specified starting pos
    void Init(string sFenStartPosition);

    //! Reset game
    void ResetMode(gamemode_t gameMode);

    //! Set timecontrol
    void SetTimecontrol(int movesToPlay, int secondsForMoves, int increment);

    //! Start game! This one start clocks & stuff...
    void StartGame();

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

    // carica un gioco ovvero una sequenza di mosse
    bool LoadGame(const char *pcGamePath);

    //! Compute the perft value for a given depth
    /*
      See: http://www.albert.nu/programs/sharper/perft.htm
    */    
    int Perft(int nDepth);
    
    //! Compute the "divide" task for the perft test
    void PerftDivide(int nDepth);

    //! Print on standard output the list of moves from a given FEN
    void PrintMovesForPosition(string sFenPosition);


    /**********************************************

      UTILITY FUNCTION MEMBERS

    ***********************************************/

    //! LOG - pulchess logging facility
    static void OpenLog(const char *pcLogFile);

    //! LOG - close the pulchess logging
    static void CloseLog();

    //! LOG - write to pulchess game log
    static void WriteLog(string &sMessage);
    
    //! IO - read an entire line from input
    static string StdinReadLine();
  };

}; // end logic namespace
}; // end pulchess namespace

#endif