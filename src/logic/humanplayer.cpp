/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: HumanPlayer implementation
 *
 * Created on 15-lug-2005
 * $Id$
 */
#include "logic/stdheader.h"

namespace pulchess { namespace logic {
	
// Class constructor
//
HumanPlayer::HumanPlayer(colour_t colour, HumanController * controller) : PlayerIF(colour)
{
	this->controller = controller;
}

// Class destructor
//
HumanPlayer::~HumanPlayer()
{
    if(controller != NULL)
		delete controller;
}

// Play a move!
//
void HumanPlayer::doYourMove()
{
	Move * coords = NULL;
	
	while(true) {
		try {
			coords = getMove();
			if( coords == NULL ) {
				cerr << "Mossa non valida" << endl;
				continue;
			}
			coords->play( _board );
			if( _board->isInCheck( getColour() ) ) {
				coords->rewind( _board );
				delete coords;
				//
				// segnala mossa non valida x' in scacco
				//
				cerr << "Mossa non valida, sei in scacco!" << endl;			
				continue;
			}
			coords->commit();
			delete coords;
			return;
		}
		catch(InvalidMoveException *e) {
			cerr << "Eccezione: mossa non valida!" << endl;
		}			  
	}
}

// Get a move out of legal ones
//
Move * HumanPlayer::getMove()
{
	CoordsMove *coords;
	list<Move *> *mList;
	list<Move *>::iterator mListIt;
	Piece      *srcp;
	
	coords = controller->getCoordsMove();
	srcp = _board->getPiece( coords->getSrcIdx() );
	
	if( srcp == NULL ) return NULL;
	
	mList = srcp->listMoves(_board);
	for(mListIt = mList->begin(); mListIt != mList->end(); mListIt++) {
		if( (*mListIt)->getSrcIdx() == coords->getSrcIdx() &&
			(*mListIt)->getDstIdx() == coords->getDstIdx() ) {
			Move * copy = (*mListIt)->copy();
			moveListDestroy(mList);
			delete mList;
			return copy;
		}
	}
	
	moveListDestroy(mList);
	delete mList;
	return NULL;
}

// Request new piece for soldier promotion
//
Piece * HumanPlayer::chooseSoldierPiece()
{
    return controller->getSoldierPiece();
}

};
};