/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:    abstract Player implementation
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

  Player::Player(colour_t colour)
  {
    _colour = colour;
    _moves  = 0;
    InitClock(40, 300);
  }

  Player::~Player()
  {
  }
  
  //
  // Get the piece colour (white or black)
  //
  colour_t Player::GetColour()
  {
    return _colour;
  }

  //
  // Am I Human? By default, no.
  //
  bool Player::IsHuman()
  {
    return false;
  }
 
  //
  // Get piece list as a delegate method
  //
  list<Piece *> * Player::GetPieceList()
  {
    return pulchess_board->ListPieces(GetColour());
  }

  //
  // Init clock
  //
  void Player::InitClock(int moves, unsigned int secondsForMoves)
  {
    _clockmoves = moves;
    _clock      = secondsForMoves;
    _clockbonus = secondsForMoves;
    _clockpushed = false;
  }

  //
  // Start thinking for a new move.
  //
  void Player::PushClock()
  {
	if(!_clockpushed)
    {
      _clockpush = time(NULL);
      _clockpushed = true;
    }
  }

  //
  // Stop clock. Increment move counter, and eventually give time bonus.
  //
  void Player::StopClock()
  {
    _clock -= (time(NULL) - _clockpush);
    _clockpushed = false;
    _moves ++;
    if( _clock > 0 && _moves%_clockmoves==0 )
    {
      _clock += _clockbonus;
    }
  }
};
};
