/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE: Pulchess Logic Facade implementation
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
#include "facade.H"
#include "book.H"

namespace pulchess { namespace logic {

//! Controller relativo al giocatore umano.
// Usa il metodo di input corrente per chiedere la mossa
class RealHumanController : public HumanController
{
	
private:
    HumanControllerFacade * controller;
    colour_t colour;
	
public:
	RealHumanController(HumanControllerFacade * c, colour_t colour)
    {
			controller = c;
			this->colour = colour;
    }
	
    CoordsMove * getCoordsMove()
    {
		string moveCmd;
		CoordsMove * move = NULL;

		try {
			moveCmd = controller->getMove();
			move = new CoordsMove(moveCmd);
		}
		catch(InvalidMoveException *e) {
			return NULL;
		}
		
		return move;
    }
    
    // return the new Piece, NULL if selected piece was not valid
    Piece * getSoldierPiece()
    {
		Piece * p;
		char piece = controller->getSoldierPiece();
		
		switch(piece) {
			
			// Regina
			case 'Q':
			case 'q':
				p = new Queen(colour);
				break;
				
				// Torre
			case 'R':
			case 'r':
				p = new Tower(colour);
				break;
				
				// Cavallo
			case 'N':
			case 'n':
				p = new Knight(colour);
				break;
				
				// Alfiere
			case 'B':
			case 'b':
				p = new Bishop(colour);
				break;
				
			default:
				break;
		}

		return p;
    }
    
};


// Facade costructor
Facade::Facade(gamemode_t gameMode)
{
    this->board    = NULL;
    this->turn     = WHITE;
    this->gameMode = gameMode;
}


// Facade destructor
Facade::~Facade()
{
    shutdown();
}


// imposta i controller
void Facade::setController(HumanControllerFacade * c, int colour)
{
    if( colour == PULCHESS_WHITE ) {
		whiteController = c;
    }
    else if( colour == PULCHESS_BLACK ) {
		blackController = c;
    }
    return;
}


// inizializza il gioco
void Facade::init()
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
			whitePlayer = new HumanPlayer(WHITE,
										  new RealHumanController(whiteController, WHITE));
			blackPlayer = new CPUPlayer(BLACK, 6, 30, true);
			break;
			
		// umano vs umano
		case HUM_VS_HUM:
			whitePlayer = new HumanPlayer(WHITE,
										  new RealHumanController(whiteController, WHITE));
			blackPlayer = new HumanPlayer(BLACK,
										  new RealHumanController(blackController, BLACK));
			break;
    }
	
    board = (void *)new Board(whitePlayer, blackPlayer);
	Book::load();	
#ifdef PULCHESS_USEHASHTABLE
	pulchess_log("[info] engine is using hashtables.");
#endif
#ifdef DEBUG
	pulchess_log("[warn] engine is in DEBUG mode!");
#endif
}


// chiude il gioco
void Facade::shutdown()
{
    Board * b = (Board *)board;
    delete b;
    board = NULL;
}


// ricostruisce la scacchiera
cellinfo_t Facade::getCellInfo(int x, int y)
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
bool Facade::isGameFinished()
{
    Board * b = (Board *)board;
    return b->isGameFinished();
}


// a chi tocca?
int Facade::whoPlaysNow()
{
    return turn;
}

// richiede di giocare a chi di turno
bool Facade::requestPlay()
{	
	bool retval;
	
    if( turn == WHITE ) {
		retval = whitePlayer->doYourMove();
    }
    else {
		retval = blackPlayer->doYourMove();
    }
	
	// la mossa e' stata effettivamente fatta, passiamo il turno
    if( retval ) turn = ENEMY(turn);
	
	return retval;
}

//! Il prossimo giocatore e' umano?
bool Facade::isHuman()
{
	if( turn == WHITE )		return whitePlayer->isHuman();
	else					return blackPlayer->isHuman();
}


// chi vince?
int Facade::gameInfo()
{
    Board * b = (Board *)board;
    return b->whoWins();
}

};
};
