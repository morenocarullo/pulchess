/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE: HumanPlayer implementation
 *
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
 *
 * Created on 15-lug-2005
 * $Id$
 */
#include "stdheader.h"

namespace pulchess { namespace logic {
	
//! Class constructor
//
HumanPlayer::HumanPlayer(colour_t colour) : Player(colour) {}

//! Class destructor
//
HumanPlayer::~HumanPlayer() {}

//! Play a move!
//
bool HumanPlayer::DoMove(string moveCmd)
{
	Move * coords = NULL;
	
	pulchess_debug("Remaining seconds in timecontrol: " << _clock << " Moves so far: " << _moves);
	
	try
	{
		coords = Move::GetMoveFactory(moveCmd, colour);
		if( coords == NULL ) {
			pulchess_info( "Mossa non valida" );
			return false;
		}
		nextPromotedPiece = getPromotion(moveCmd);
		coords->Play();
		if( pulchess_board->IsInCheck( colour ) ) {
			coords->Rewind();
			delete coords;
			pulchess_info( "Mossa non valida, vai/sei in scacco!" );			
			return false;
		}
		pulchess_board->MoveFinalize(coords);
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

//! Request new piece for soldier promotion
//
Piece * HumanPlayer::ChoosePawnPiece()
{
    // TODO: check all legal cases...
    switch(nextPromotedPiece)
    {
      case PIECE_BISHOP:
        return new Bishop(colour);
        break;
      case PIECE_ROOK:
        return new Rook(colour);
        break;
      case PIECE_KNIGHT:
        return new Knight(colour);
        break;
    }
    return new Queen(colour);
}

bool HumanPlayer::IsHuman()
{
	return true;
}

};
};
