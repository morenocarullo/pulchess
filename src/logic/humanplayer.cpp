/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE: HumanPlayer implementation
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

namespace pulchess { namespace logic {
	
//! Class constructor
//
HumanPlayer::HumanPlayer(colour_t colour, HumanController * controller) : PlayerIF(colour)
{
	this->controller = controller;
}

//! Class destructor
//
HumanPlayer::~HumanPlayer()
{
    if(controller != NULL)
		delete controller;
}

//! Play a move!
//
bool HumanPlayer::doYourMove()
{
	Move * coords = NULL;
	
	try {
		coords = getMove();
		if( coords == NULL ) {
			cerr << "Mossa non valida" << endl;
			return false;
		}
		coords->play( _board );
		if( _board->isInCheck( getColour() ) ) {
			coords->rewind( _board );
			delete coords;
			//
			// segnala mossa non valida x' in scacco
			//
			cerr << "Mossa non valida, sei in scacco!" << endl;			
			return false;
		}
		coords->commit();
		delete coords;
		return true;
	}
	catch(InvalidMoveException *e) {
		cerr << "Eccezione: mossa non valida!" << endl;
	}			  
	
	return false;
}

//! Get a move out of legal ones
//
Move * HumanPlayer::getMove()
{
	CoordsMove *coords;
	list<Move *> mList;
	list<Move *>::iterator mListIt;
	Piece      *srcp;
	
	// errore: non e' stata data una mossa valida (sintatticamente)
	if( (coords = controller->getCoordsMove()) == NULL ) 
		return NULL;
	
	srcp = _board->getPiece( coords->getSrcIdx() );
	
	// errore: qui non c'e' nessun pezzo
	if( srcp == NULL ) return NULL;
	
	// errore: non possiamo muovere pezzi di altri!
	if( srcp->getColour() != this->getColour() ) return NULL;
	
	srcp->listMoves(_board, &mList);
	for(mListIt = mList.begin(); mListIt != mList.end(); mListIt++) {
		if( (*mListIt)->getSrcIdx() == coords->getSrcIdx() &&
			(*mListIt)->getDstIdx() == coords->getDstIdx() ) {
			Move * copy = (*mListIt)->copy();
			moveListDestroy(&mList);
			return copy;
		}
	}
	
	moveListDestroy(&mList);
	return NULL;
}

//! Request new piece for soldier promotion
//
Piece * HumanPlayer::chooseSoldierPiece()
{
    return controller->getSoldierPiece();
}

bool HumanPlayer::isHuman()
{
	return true;
}

};
};
