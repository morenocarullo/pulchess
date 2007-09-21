/*
  PulCHESS, a Computer Chess program
                by Moreno Carullo
 
  About this file:
         This file contains the main pulchess facade (implementation),  
         used to expose to other programs the pulchess logic.
 
  License:
         GPL v2, see license.txt in project root.
 
  Version:
         $Id$

*/
#include "stdheader.h"
#include "pulchess.H"
#include "book.H"
#include <sstream>

/** pulchess static local vars */
static const char version[] = "PulCHESS 0.2f";

/** pulchess lobal vars */
bool pulchess_log_on = true;

namespace pulchess {
	namespace logic {

// Pulchess costructor
Pulchess::Pulchess(gamemode_t gameMode)
{
    this->gameMode = gameMode;
	this->engineStatus = PULCHESS_STATUS_ZERO;
}


// Pulchess destructor
Pulchess::~Pulchess()
{
    Shutdown();
}

//
// Get a string containing engine's name and version
//
string Pulchess::GetPulchessVersion()
{
	return string(version);
}

//
// Change game mode
//
void Pulchess::ResetMode(gamemode_t gameMode)
{
  this->gameMode = gameMode;
  this->engineStatus = PULCHESS_STATUS_ZERO;
  if( Board::board != NULL )
  {
	delete Board::board;
	Board::board = NULL;	 
  }
  Init();
}

//
// Set time control. This has to be called before game starts.
//
//  In conventional clock mode, increment=0.
//  In increment mode, movesToPlay=0, secondsForMoves is the base.
//  In exact seconds for move, secondsForMoves is the only positive number.
//
void Pulchess::SetTimecontrol(int movesToPlay, int secondsForMoves, int increment)
{
  pulchess_debug("Set time control to:" << movesToPlay << "," << secondsForMoves << "," << increment);

  if( increment == 0 && movesToPlay>0 && secondsForMoves > 0) {
   	pulchess_debug("Conventional clock mode: " << movesToPlay << " moves in " << secondsForMoves << " s.");
  }
  else if( movesToPlay == 0 && increment>0 && secondsForMoves > 0) {
    pulchess_debug("Increment clock mode: base " << secondsForMoves << " s and increment " << increment << " s." );
  }
  else if( movesToPlay == 0 && increment == 0 && secondsForMoves > 0) {
    pulchess_debug("Exact seconds clock mode: " << secondsForMoves << " s for move." );
  }

  if( pulchess_the_white != NULL & pulchess_the_black != NULL ) {
    pulchess_the_white->InitClock(movesToPlay, secondsForMoves, increment);
    pulchess_the_black->InitClock(movesToPlay, secondsForMoves, increment);
  }
}

//
// Init engine
//
void Pulchess::Init()
{
	const int DEFAULT_MAXPLY = 6;
	
    // seleziona la modalita' di gioco.
    switch( gameMode )
	{
		// computer vs computer
		case CPU_VS_CPU:
			pulchess_the_white = new CPUPlayer(WHITE, DEFAULT_MAXPLY, 30, true);
			pulchess_the_black = new CPUPlayer(BLACK, DEFAULT_MAXPLY, 30, true);
			break;
	
		// umano vs computer
		case HUM_VS_CPU:
			pulchess_the_white = new HumanPlayer(WHITE);
			pulchess_the_black = new CPUPlayer(BLACK, DEFAULT_MAXPLY, 30, true);
			break;
			
		// umano vs computer
		case CPU_VS_HUM:
			pulchess_the_white = new CPUPlayer(WHITE, DEFAULT_MAXPLY, 30, true);
			pulchess_the_black = new HumanPlayer(BLACK);
			break;
			
		// umano vs umano
		case HUM_VS_HUM:
			pulchess_the_white = new HumanPlayer(WHITE);
			pulchess_the_black = new HumanPlayer(BLACK);
			break;
    }
	
    Board::board = new Board();
	engineStatus = PULCHESS_STATUS_INIT;
	Book::Load();
	
	pulchess_debug("pulchess is in DEBUG mode!");
#ifdef PULCHESS_USEHASHTABLE
	pulchess_debug("pulchess is using hashtables.");
#endif
}

// carica un gioco
bool Pulchess::loadGame(const char *gamePath)
{
	// crea una classe "controller" che carica le mosse da file
	bool endLoading = false;
	
	FILE *fp  = fopen(gamePath, "rt");
	if( fp == NULL )
	{
		pulchess_error("error opening game file: " << gamePath);
		return false;
	}
	
	pulchess_the_white  = new HumanPlayer(WHITE);
	pulchess_the_black  = new HumanPlayer(BLACK);
    pulchess_board      = new Board();
    engineStatus        = PULCHESS_STATUS_INIT;							
										  
    // TODO: read from file game mode
    //       read from file timings
	
	while(!feof(fp) && !endLoading)
	{
		string mossa;
		char buff[6];
		fscanf(fp, "%s", buff);
		mossa = buff;
		
		if(!gameCommand(mossa))
		{
		  pulchess_error("error loading game file: " << gamePath << " at char " << ftell(fp));
		  fclose(fp);
		  return false;
		}
	}
	
	fclose(fp);
	return true;
}

//
// Start game
//
void Pulchess::StartGame()
{
	if( engineStatus & PULCHESS_STATUS_INIT )
    {
      pulchess_the_white->PushClock();
      engineStatus |= PULCHESS_STATUS_START;
    }
}

//
// Close game
//
void Pulchess::Shutdown()
{
    delete pulchess_board;
    pulchess_board = NULL;
}

//
// Ricostruisce la scacchiera
//
cellinfo_t Pulchess::getCellInfo(int x, int y)
{
    Piece * p;
    cellinfo_t c;
	
    p        = pulchess_board->GetPiece(x,y);
	
    if( p != NULL ) {
		c.kind   = p->getKindChr();
		c.colour = p->getColourChr();
    } else {
		c.kind   = '-';
		c.colour = 0;
    }
	
    return c;
}

//
// Is game ended?
//
bool Pulchess::IsGameFinished()
{
    return (pulchess_board != NULL) ? pulchess_board->IsGameFinished() : true;
}

//
// Who has to move now?
//
int Pulchess::whoPlaysNow()
{
    return (pulchess_board != NULL)  ? pulchess_board->turn : PULCHESS_WHITE;
}

//
// Accept a move command from current player
//
bool Pulchess::gameCommand(string &cmd)
{	
	bool retval;
	
	if( ! (engineStatus & PULCHESS_STATUS_INIT) )
    {
      pulchess_error("pulchess:gameCommand called before init");
      return false;
    }
	
    if( pulchess_board->turn == WHITE ) {
		retval = pulchess_the_white->DoMove(cmd);
    }
    else {
		retval = pulchess_the_black->DoMove(cmd);
    }
	
	return retval;
}

//
// Play an empty move
//
bool Pulchess::gameCommand()
{
  string cmd = "";
  return gameCommand(cmd);
}

// La stringa data e' una mossa?
// da spostare nella classe Move come membro statico
bool Pulchess::IsMove(string &cmd)
{
  if( cmd == "O-O" )   return true;
  if( cmd == "O-O-O" ) return true;
  if( cmd[0] <= 'h' && cmd[0] >= 'a' &&
      cmd[2] <= 'h' && cmd[2] >= 'a' && 
      cmd[1] <= '8' && cmd[1] >= '1' &&
      cmd[3] <= '8' && cmd[3] >= '1' )
  {
    return true;	
  }

  return false;
}

// Il prossimo giocatore e' umano?
bool Pulchess::IsHuman()
{
  if( engineStatus == PULCHESS_STATUS_ZERO ) {
	return false;
  }
  if( pulchess_board->turn == WHITE ) {
    return pulchess_the_white->IsHuman();
  }
  else {
    return pulchess_the_black->IsHuman();
  }
}

//
// Is the specified player human?
//
bool Pulchess::IsHuman(const colour_t colour)
{
	if( engineStatus == PULCHESS_STATUS_ZERO ) return false;
	return (colour == WHITE) ? pulchess_the_white->IsHuman() : pulchess_the_black->IsHuman(); 
}

//
// Gimme last played move
//
Move * Pulchess::GetLastMove()
{
	return (pulchess_board == NULL) ? NULL : pulchess_board->GetLastMove();
}

//
// Game's move report
//
string  Pulchess::GetGameMovesReport()
{
  string report = "";
  list<Move *> * mlist;
  list<Move *>::iterator it;
     int ct;	

  if( pulchess_board == NULL ) return report;

  mlist = pulchess_board->GetMoveList();

  for (ct = 0, it = mlist->begin(); it != mlist->end(); it++, ct++)
  {
    if( ct%2 == 0)
    {
	  stringstream strs;
	  string       str;
	  strs << (ct/2+1);
	  strs >> str;
	  report += str;
      report += ". ";
    }
    report += (*it)->toString();
    report += " ";
  }
  return report;
}

//
// Game status
//
int Pulchess::gameInfo()
{
    return pulchess_board->WhoWins();
}

//
// Print board.
// TODO: refactor this into Board
//
void Pulchess::printBoard()
{
  if( pulchess_board != NULL ) {
    pulchess_board->Print();
  }
}

};
};