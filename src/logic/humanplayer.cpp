/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
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
HumanPlayer::HumanPlayer(colour_t colour) : Player(colour)
{
}

//! Class destructor
//
HumanPlayer::~HumanPlayer()
{
}

//! Play a move!
//
bool HumanPlayer::DoMove(string moveCmd)
{
	Move * coords = NULL;
	
	pulchess_debug("Remaining seconds in timecontrol: " << _clock << " Moves so far: " << _moves);
	
	try
	{
		coords = getMove(moveCmd);
		if( coords == NULL ) {
			pulchess_info( "Mossa non valida" );
			return false;
		}
		nextPromotedPiece = getPromotion(moveCmd);
		coords->Play( _board );
		if( _board->IsInCheck( GetColour() ) ) {
			coords->Rewind( _board );
			delete coords;
			pulchess_info( "Mossa non valida, vai/sei in scacco!" );			
			return false;
		}
		_board->MoveFinalize(coords);
		nextPromotedPiece = PIECE_QUEEN;
		return true;
	}
	catch(InvalidMoveException *e)
	{
		pulchess_info( "Eccezione: mossa non valida!" );
	}			  
	
	return false;
}

int HumanPlayer::getPromotion(string moveCmd)
{
	if(moveCmd.length() != 5) return PIECE_NONE;
	{
		switch(moveCmd[4])
		{
	      case 'b':
	      case 'B':
	        return PIECE_BISHOP;
	        break;
	      case 'r':
	      case 'R':
	        return PIECE_ROOK;
	        break;
	      case 'n':
	      case 'N':
	        return PIECE_KNIGHT;
		}
	}
	return PIECE_NONE;
}

//! Get a move out of legal ones
//
Move * HumanPlayer::getMove(string moveCmd)
{
	CoordsMove *coords;
	vector<Move *> mList;
	vector<Move *>::iterator mListIt;
	Piece      *srcp;
	
    // interpreta la mossa
	try {
	   coords = new CoordsMove(moveCmd);
    }
    catch(InvalidMoveException *e)
    {
       pulchess_info("la mossa specificata non e' valida");
	   return NULL;
	}
	
	srcp = _board->GetPiece( coords->GetSrcIdx() );
	
	// errore: qui non c'e' nessun pezzo
	if( srcp == NULL ) return NULL;
	
	// errore: non possiamo muovere pezzi di altri!
	if( srcp->GetColour() != this->GetColour() ) return NULL;
	
	srcp->listMoves(_board, &mList);
	for(mListIt = mList.begin(); mListIt != mList.end(); mListIt++) {
		if( (*mListIt)->GetSrcIdx() == coords->GetSrcIdx() &&
			(*mListIt)->GetDstIdx() == coords->GetDstIdx() ) {
			Move * copy = (*mListIt)->copy();
			moveListDestroy(&mList);
			return copy;
		}
	}
	
	pulchess_debug( "HumanPlayer::getMove() -- la mossa specificata non"
                    << " e' stata trovata tra quelle valide: " << coords->toString() );

	for(mListIt = mList.begin(); mListIt != mList.end(); mListIt++) {
		pulchess_debug( "\tmossa disp: " << (*mListIt)->toString() );
	}
	
	moveListDestroy(&mList);
	return NULL;
}

//! Request new piece for soldier promotion
//
Piece * HumanPlayer::ChoosePawnPiece()
{
    // TODO: check all legal cases...
    switch(nextPromotedPiece)
    {
      case PIECE_BISHOP:
        return new Bishop(GetColour());
        break;
      case PIECE_ROOK:
        return new Rook(GetColour());
        break;
      case PIECE_KNIGHT:
        return new Knight(GetColour());
        break;
    }
    return new Queen(GetColour());
}

bool HumanPlayer::IsHuman()
{
	return true;
}

};
};