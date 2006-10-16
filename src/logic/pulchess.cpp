/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE: Pulchess Logic Facade class
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

/** pulchess engine global vars */
bool pulchess_log_on = true;

namespace pulchess { namespace logic {

// Pulchess costructor
Pulchess::Pulchess(gamemode_t gameMode)
{
    this->board    = NULL;
    this->turn     = WHITE;
    this->gameMode = gameMode;
	this->engineStatus = PULCHESS_STATUS_ZERO;
}


// Pulchess destructor
Pulchess::~Pulchess()
{
    shutdown();
}


// inizializza il gioco
void Pulchess::init()
{
    // seleziona la modalita' di gioco.
    switch( gameMode )
	{
		// computer vs computer
		case CPU_VS_CPU:
			whitePlayer = new CPUPlayer(WHITE, 6, 30, true);
			blackPlayer = new CPUPlayer(BLACK, 6, 30, true);
			break;
	
		// umano vs computer
		case HUM_VS_CPU:
			whitePlayer = new HumanPlayer(WHITE);
			blackPlayer = new CPUPlayer(BLACK, 6, 30, true);
			break;
			
		// umano vs computer
		case CPU_VS_HUM:
			whitePlayer = new CPUPlayer(WHITE, 6, 30, true);
			blackPlayer = new HumanPlayer(BLACK);
			break;
			
		// umano vs umano
		case HUM_VS_HUM:
			whitePlayer = new HumanPlayer(WHITE);
			blackPlayer = new HumanPlayer(BLACK);
			break;
    }
	
    board = (void *)new Board(whitePlayer, blackPlayer);
	engineStatus = PULCHESS_STATUS_INIT;
	Book::load();
	
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
    board        = (void *)new Board(whitePlayer, blackPlayer);
    engineStatus = PULCHESS_STATUS_INIT;							
										  
    // TODO: read from file game mode
	
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


// chiude il gioco
void Pulchess::shutdown()
{
    Board * b = (Board *)board;
    delete b;
    board = NULL;
}


// ricostruisce la scacchiera
cellinfo_t Pulchess::getCellInfo(int x, int y)
{
    Board * b = (Board *)board;
    Piece * p;
    cellinfo_t c;
	
    p        = b->getPiece(x,y);
	
    if( p != NULL ) {
		c.kind   = p->getKindChr();
		c.colour = p->getColourChr();
    } else {
		c.kind   = '-';
		c.colour = 0;
    }
	
    return c;
}


// il gioco e' finito?
bool Pulchess::isGameFinished()
{
    Board * b = (Board *)board;
    return b->isGameFinished();
}


// a chi tocca?
int Pulchess::whoPlaysNow()
{
    return turn;
}

// richiede di giocare a chi di turno
bool Pulchess::gameCommand(string cmd)
{	
	bool retval;
	
	if( ! (engineStatus & PULCHESS_STATUS_INIT) )
    {
      pulchess_error("pulchess:gameCommand called before init");
      return false;
    }
	
    if( turn == WHITE ) {
		retval = whitePlayer->doMove(cmd);
    }
    else {
		retval = blackPlayer->doMove(cmd);
    }
	
	// la mossa e' stata effettivamente fatta, passiamo il turno
    if( retval ) turn = ENEMY(turn);
	
	return retval;
}

//! Il prossimo giocatore e' umano?
bool Pulchess::isHuman()
{
	if( turn == WHITE )		return whitePlayer->isHuman();
	else					return blackPlayer->isHuman();
}


// chi vince?
int Pulchess::gameInfo()
{
    Board * b = (Board *)board;
    return b->whoWins();
}

void Pulchess::printBoard()
{
  cout << "" << endl;
  for(int y=7; y>=0; y--) {
    printf("%d   ", y+1);
    for(int x=0; x<8; x++) {
      cellinfo_t c = getCellInfo(x,y);
      if( c.colour == 'w') {
	cout << (char)toupper(c.kind);
      }
      else {
	cout << c.kind;
      }
      cout << " ";
    }
    cout << endl;
  }

  // intestazione delle colonne
  cout << endl << "    ";
  for(int x=0; x<8; x++) {
    printf("%c ", 'a'+x);
  }
  cout << "" << endl << endl;
}

};
};
