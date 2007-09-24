/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:    abstract Player implementation
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
	
  //
  // Global white player instance
  Player* Player::whitePlayer = NULL;

  //
  // Global Timecontrol option. This sets the default number of moves.
  unsigned int Player::ClockDefaultMoves = 40;

  //
  // Global Timecontrol option. This sets the default seconds for each move.
  unsigned int Player::ClockDefaultSeconds = 300;

  //
  // Global Timecontrol option. This sets the default increment.
  unsigned int Player::ClockDefaultIncr = 0;

  //
  // Global black player instance
  Player* Player::blackPlayer = NULL;

  Player::Player(colour_t colour)
  {
    this->colour = colour;
    this->_moves  = 0;
    InitClock(Player::ClockDefaultMoves,
              Player::ClockDefaultSeconds,
              Player::ClockDefaultIncr);
  }

  Player::~Player()
  {
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
    return pulchess_board->ListPieces(colour);
  }

  //
  // Init clock
  //
  void Player::InitClock(int moves, unsigned int secondsForMoves, unsigned int incrSeconds)
  {
    _clockmoves = moves;
    _clock      = secondsForMoves;
    _clockbonus = secondsForMoves;
    _clockincr   = incrSeconds;
    _clockpushed = false;

    // Save clock options
	Player::ClockDefaultMoves = moves;
    Player::ClockDefaultSeconds = secondsForMoves;
    Player::ClockDefaultIncr = incrSeconds;
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
    _clock += _clockincr;
    _clockpushed = false;
    _moves ++;
    if( _clock > 0 && _clockmoves>0 && _moves%_clockmoves==0 )
    {
      _clock += _clockbonus;
    }
  }
};
};
