/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Pulchess Logic Facade class
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
#include "stdheader.h"
#include "pulchess.H"
#include "book.H"
#include <sstream>

/** pulchess static local vars */
static const char version[] = "PulCHESS 0.2b";

/** pulchess lobal vars */
bool pulchess_log_on = true;

namespace pulchess { namespace logic {

// Pulchess costructor
Pulchess::Pulchess(gamemode_t gameMode)
{
    this->board    = NULL;
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
  if( board != NULL )
  {
	delete board;	 
  }
  Init();
}

//
// Set time control. This has to be called before game starts.
//
void Pulchess::SetTimecontrol(int movesToPlay, int secondsForMoves)
{
  if( whitePlayer != NULL & blackPlayer != NULL )
  {
    whitePlayer->InitClock(movesToPlay, secondsForMoves);
    blackPlayer->InitClock(movesToPlay, secondsForMoves);
  }
}

//
// Init engine
//
void Pulchess::Init()
{
	const int DEFAULT_MAXPLY = 30;
	
    // seleziona la modalita' di gioco.
    switch( gameMode )
	{
		// computer vs computer
		case CPU_VS_CPU:
			whitePlayer = new CPUPlayer(WHITE, DEFAULT_MAXPLY, 30, true);
			blackPlayer = new CPUPlayer(BLACK, DEFAULT_MAXPLY, 30, true);
			break;
	
		// umano vs computer
		case HUM_VS_CPU:
			whitePlayer = new HumanPlayer(WHITE);
			blackPlayer = new CPUPlayer(BLACK, DEFAULT_MAXPLY, 30, true);
			break;
			
		// umano vs computer
		case CPU_VS_HUM:
			whitePlayer = new CPUPlayer(WHITE, DEFAULT_MAXPLY, 30, true);
			blackPlayer = new HumanPlayer(BLACK);
			break;
			
		// umano vs umano
		case HUM_VS_HUM:
			whitePlayer = new HumanPlayer(WHITE);
			blackPlayer = new HumanPlayer(BLACK);
			break;
    }
	
    board = new Board(whitePlayer, blackPlayer);
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
	
	whitePlayer  = new HumanPlayer(WHITE);
	blackPlayer  = new HumanPlayer(BLACK);
    board        = new Board(whitePlayer, blackPlayer);
    engineStatus = PULCHESS_STATUS_INIT;							
										  
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
      whitePlayer->PushClock();
      engineStatus |= PULCHESS_STATUS_START;
    }
}

//
// Close game
//
void Pulchess::Shutdown()
{
    delete board;
    board = NULL;
}

//
// Ricostruisce la scacchiera
//
cellinfo_t Pulchess::getCellInfo(int x, int y)
{
    Piece * p;
    cellinfo_t c;
	
    p        = board->GetPiece(x,y);
	
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
    return board->IsGameFinished();
}

//
// Who has to move now?
//
int Pulchess::whoPlaysNow()
{
    return (board != NULL)  ? board->turn : PULCHESS_WHITE;
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
	
    if( board->turn == WHITE ) {
		retval = whitePlayer->DoMove(cmd);
    }
    else {
		retval = blackPlayer->DoMove(cmd);
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
	if( engineStatus == PULCHESS_STATUS_ZERO ) return false;
	if( board->turn == WHITE ) return whitePlayer->IsHuman();
	else					   return blackPlayer->IsHuman();
}

//
// Is the specified player human?
//
bool Pulchess::IsHuman(const colour_t colour)
{
	if( engineStatus == PULCHESS_STATUS_ZERO ) return false;
	return (colour == WHITE) ? whitePlayer->IsHuman() : blackPlayer->IsHuman(); 
}

//
// Gimme last played move
//
Move * Pulchess::GetLastMove()
{
	return (board == NULL) ? NULL : board->GetLastMove();
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

  if( board == NULL ) return report;

  mlist = board->GetMoveList();

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
    return board->WhoWins();
}

//
// Print board.
// TODO: refactor this into Board
//
void Pulchess::printBoard()
{
  cout << "" << endl;
  for(int y=7; y>=0; y--)
  {
    printf("%d   ", y+1);
    for(int x=0; x<8; x++)
    {
      cellinfo_t c = getCellInfo(x,y);
      if( c.colour == 'w')
      {
        cout << (char)toupper(c.kind);
      }
      else
      {
        cout << c.kind;
      }
      cout << " ";
    }
    cout << endl;
  }

  cout << endl << "    ";
  for(int x=0; x<8; x++) {
    printf("%c ", 'a'+x);
  }
  cout << "" << endl << endl;
}

};
};