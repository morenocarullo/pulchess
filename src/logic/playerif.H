/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:    abstract Player interface
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
 * $Id$
 */
#include "stdheader.h"

#ifndef _PULCHESS_PLAYERIF_H_
#define _PULCHESS_PLAYERIF_H_

namespace pulchess { namespace logic {

  //! Chess player abstract class
  class Player {

  protected:
    Board*   _board;
    colour_t _colour;

    //! Remaining time
    time_t   _clock;

    //! Bonus time after _clockmoves moves
    time_t   _clockbonus;

    //! When clock was pushed last
    time_t   _clockpush;

    //! Is clock pushed? This is used to prevent double-pushing
    bool     _clockpushed;

    //! Moves in a timecontrol block
    int      _clockmoves;

    //! Moves so far
    int      _moves;

  public:
    Player();
    Player(colour_t colour);
    virtual ~Player();
    
    //! Which is my colour?
    colour_t GetColour();

    //! Which piece should I promote the pawn to?
    virtual Piece * ChoosePawnPiece() = 0;

    //! Am i a human player or an AI one?
    virtual bool IsHuman();	

    //! Play a (specified) move
    virtual bool DoMove(string moveCmd) = 0;

    //! Set the chess board
    void SetBoard(Board* b);

    //! Get the piece list for this player
    list<Piece *> * GetPieceList();

    //! Init clock
    void InitClock(int moves, int secondsForMoves);

    //! Push the clock, start thinking a move
    void PushClock();

    //! Stop clock, increment move count
    void StopClock();
  };
  
};
};
#endif