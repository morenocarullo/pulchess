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

  public:
    colour_t colour;

  protected:

	static unsigned int ClockDefaultMoves;
    static unsigned int ClockDefaultSeconds;
    static unsigned int ClockDefaultIncr;

    //! Remaining time
    unsigned int   _clock;

    //! Bonus time after _clockmoves moves
    unsigned int   _clockbonus;

    //! When clock was pushed last
    time_t   _clockpush;

    //! Is clock pushed? This is used to prevent double-pushing
    bool     _clockpushed;

    //! Moves in a timecontrol block
    int      _clockmoves;

    //! Clock increment for each move
    int      _clockincr;

    //! Moves so far
    int      _moves;

  public:

    //! Pointer to the (global) white Player
    static Player* whitePlayer;

    //! Pointer to the (global) black Player
    static Player* blackPlayer;


    Player();
    Player(colour_t colour);
    
    //! Destructor
    virtual ~Player();
    
    //! Which piece should I promote the pawn to?
    virtual Piece * ChoosePawnPiece() = 0;

    //! Am i a human player or an AI one?
    virtual bool IsHuman();	

    //! Play a (specified) move
    virtual bool DoMove(string moveCmd) = 0;

    //! Get the piece list for this player
    list<Piece *> * GetPieceList();

    //! Init clock (conventional mode)
    void InitClock(int moves, unsigned int secondsForMoves, unsigned int incrSeconds);

    //! Push the clock, start thinking a move
    void PushClock();

    //! Stop clock, increment move count
    void StopClock();
  };
  
};
};
#endif

/*
	(MC) Until not fully implemented, here are the details of time controls:
	
xboard supports three styles of time control: conventional chess clocks, the ICS-style incremental clock,
and an exact number of seconds per move.

In conventional clock mode, every time control period is the same. That is, if the time control is 40 moves
in 5 minutes, then after each side has made 40 moves, they each get an additional 5 minutes, and so on, ad infinitum.
The command to set a conventional time control looks like this:

	level 40 5 0
	level 40 0:30 0
		
The 40 means that there are 40 moves per time control. The 5 means there are 5 minutes in the control.
In the second example, the 0:30 means there are 30 seconds. The final 0 means that we are in conventional clock mode.

The command to set an incremental time control looks like this:

	level 0 2 12
		
Here the 0 means "play the whole game in this time control period", the 2 means "base=2 minutes",
and the 12 means "inc=12 seconds". As in conventional clock mode, the second argument to level can
be in minutes and seconds.

At the start of the game, each player's clock is set to base minutes. Immediately after a player makes a move, inc seconds are added to his clock. A player's clock counts down while it is his turn. Your flag can be called whenever your clock is zero or negative. (Your clock can go negative and then become positive again because of the increment.)
A special rule on some ICS implementations: if you ask for a game with base=0, the clocks really start at 10 seconds instead of 0.

The command to set an exact number of seconds per move looks like this:

	st 30
		
This means that each move must be made in at most 30 seconds.
Time not used on one move does not accumulate for use on later moves.
*/
