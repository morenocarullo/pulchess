/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE:	   XBoard interface
* 
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
*
* Created on 20-giu-2006
* $Id$
*/
#include "xboard.H"
#include "pulchess.H"
#include <iostream>

using namespace std;
using namespace pulchess::logic;

namespace pulchess {

XBoard::XBoard()
{
}

XBoard::~XBoard()
{
}
	
bool
XBoard::readCommand(const char * cmd)
{
	return false;
}

void
XBoard::unknownCommand(const char * cmd)
{
}

void
XBoard::unimplCommand(const char *cmd)
{

}

void
XBoard::mainLoop()
{
  char buff[255];
  run = true;
  
  while(readCommand(buff) && run)
  {
    switch(buff[0])
    {
      case 'n': // new
    	  if( strcmp(buff, "new") != 0)
    	  {
    	    unknownCommand(buff);
    	    continue;
    	  }
    	  //Pulchess * facade = new Pulchess(HUM_VS_CPU);
    	  break;

      case 'v': // variant
	  unimplCommand(buff);
	  break;

      case 'q': // quit
	  if( strcmp(buff, "quit") != 0)
	  {
	    unknownCommand(buff);
	    continue;
	  }
	  run = false;
	  break;


      case 'r': // gioco randomizzato
	  if( strcmp(buff, "random") != 0)
	  {
	    unknownCommand(buff);
	    continue;
	  }
          unimplCommand(buff);	  
	  break;

	  
      case 'f': // force mode	  
	 /* 
	  * con questo comando il motore dovrebbe
	  * solo controllare la correttezza delle mosse.
	  */
	 if( strcmp(buff, "force") != 0)
	 {
	   unknownCommand(buff);
	   continue;
	 }
	 unimplCommand(buff);
	 break;

	 
      case 'g': // go
	 /*
	  * Leave force mode and set the engine to play the color that is on move.
	  * Associate the engine's clock with the color that is on move,
	  * the opponent's clock with the color that is not on move.
	  * Start the engine's clock. Start thinking and eventually make a move.
	  */
	 if( strcmp(buff, "go") != 0)
	 {
	   unknownCommand(buff);
	   continue;
	 }
	 //game->startClock();
	 //game->playMove();
	 break;

      case 'w':
	 /*
	  * Set White on move. Set the engine to play Black. Stop clocks.
	  */
	 if( strcmp(buff, "white") != 0)
	 {
	   unknownCommand(buff);
	   continue;
	 }	 
	 break;

      case 'b':
         /*
	  * Set Black on move. Set the engine to play White. Stop clocks.
	  */
	 if( strcmp(buff, "black") != 0)
	 {
	   unknownCommand(buff);
	   continue;
	 }
	 break;

      case 'l':
	 break;

      case '?':
         /*
	  * "Move now" - comando che si puo' ignorare.
	  */
	 break;

      case 's':
	 /*
	  * sd :
	  *
	  * st :
	  */
	 break;
    }
  }
}


/*
level MPS BASE INC
    Set time controls. See the Time Control section below.

st TIME
    Set time controls. See the Time Control section below. The commands "level" and "st" are not used together.

sd DEPTH
    The engine should limit its thinking to DEPTH ply.

time N
    Set a clock that always belongs to the engine. N is a number in centiseconds (units of 1/100 second). Even if the engine changes to playing the opposite color, this clock remains with the engine.

otim N
    Set a clock that always belongs to the opponent. N is a number in centiseconds (units of 1/100 second). Even if the opponent changes to playing the opposite color, this clock remains with the opponent.

    If needed for purposes of board display in force mode (where the engine is not participating in the game) the time clock should be associated with the last color that the engine was set to play, the otim clock with the opposite color.

    Beginning in protocol version 2, if you can't handle the time and otim commands, you can use the "feature" command to disable them; see below. The following techniques from older protocol versions also work: You can ignore the time and otim commands (that is, treat them as no-ops), or send back "Error (unknown command): time" the first time you see "time".
MOVE
    See below for the syntax of moves. If the move is illegal, print an error message; see the section "Commands from the engine to xboard". If the move is legal and in turn, make it. If not in force mode, stop the opponent's clock, start the engine's clock, start thinking, and eventually make a move.

    When xboard sends your engine a move, it normally sends coordinate algebraic notation. Examples:

    Normal moves:	e2e4
    Pawn promotion:	e7e8q
    Castling:	e1g1, e1c1, e8g8, e8c8
    Bughouse/crazyhouse drop:	P@h3
    ICS Wild 0/1 castling:	d1f1, d1b1, d8f8, d8b8
    FischerRandom castling:	O-O, O-O-O (oh, not zero)

    Beginning in protocol version 2, you can use the feature command to select SAN (standard algebraic notation) instead; for example, e4, Nf3, exd5, Bxf7+, Qxf7#, e8=Q, O-O, or P@h3. Note that the last form, P@h3, is a extension to the PGN standard's definition of SAN, which does not support bughouse or crazyhouse.

    xboard doesn't reliably detect illegal moves, because it does not keep track of castling unavailability due to king or rook moves, or en passant availability. If xboard sends an illegal move, send back an error message so that xboard can retract it and inform the user; see the section "Commands from the engine to xboard".
usermove MOVE
    By default, moves are sent to the engine without a command name; the notation is just sent as a line by itself. Beginning in protocol version 2, you can use the feature command to cause the command name "usermove" to be sent before the move. Example: "usermove e2e4".


    It is also permissible for your engine to move immediately if it gets any command while thinking, as long as it processes the command right after moving, but it's preferable if you don't do this. For example, xboard may send post, nopost, easy, hard, force, quit, or other commands while the engine is on move.
ping N
    In this command, N is a decimal number. When you receive the command, reply by sending the string pong N, where N is the same number you received. Important: You must not reply to a "ping" command until you have finished executing all commands that you received before it. Pondering does not count; if you receive a ping while pondering, you should reply immediately and continue pondering. Because of the way xboard uses the ping command, if you implement the other commands in this protocol, you should never see a "ping" command when it is your move; however, if you do, you must not send the "pong" reply to xboard until after you send your move. For example, xboard may send "?" immediately followed by "ping". If you implement the "?" command, you will have moved by the time you see the subsequent ping command. Similarly, xboard may send a sequence like "force", "new", "ping". You must not send the pong response until after you have finished executing the "new" command and are ready for the new game to start.

    The ping command is new in protocol version 2 and will not be sent unless you enable it with the "feature" command. Its purpose is to allow several race conditions that could occur in previous versions of the protocol to be fixed, so it is highly recommended that you implement it. It is especially important in simple engines that do not ponder and do not poll for input while thinking, but it is needed in all engines.
draw
    The engine's opponent offers the engine a draw. To accept the draw, send "offer draw". To decline, ignore the offer (that is, send nothing). If you're playing on ICS, it's possible for the draw offer to have been withdrawn by the time you accept it, so don't assume the game is over because you accept a draw offer. Continue playing until xboard tells you the game is over. See also "offer draw" below.

result RESULT {COMMENT}
    After the end of each game, xboard will send you a result command. You can use this command to trigger learning. RESULT is either 1-0, 0-1, 1/2-1/2, or *, indicating whether white won, black won, the game was a draw, or the game was unfinished. The COMMENT string is purely a human-readable comment; its content is unspecified and subject to change. In ICS mode, it is passed through from ICS uninterpreted. Example:

    result 1-0 {White mates}

    Here are some notes on interpreting the "result" command. Some apply only to playing on ICS ("Zippy" mode).

    If you won but did not just play a mate, your opponent must have resigned or forfeited. If you lost but were not just mated, you probably forfeited on time, or perhaps the operator resigned manually. If there was a draw for some nonobvious reason, perhaps your opponent called your flag when he had insufficient mating material (or vice versa), or perhaps the operator agreed to a draw manually.

    You will get a result command even if you already know the game ended -- for example, after you just checkmated your opponent. In fact, if you send the "RESULT {COMMENT}" command (discussed below), you will simply get the same thing fed back to you with "result" tacked in front. You might not always get a "result *" command, however. In particular, you won't get one in local chess engine mode when the user stops playing by selecting Reset, Edit Game, Exit or the like.
setboard FEN
    The setboard command is the new way to set up positions, beginning in protocol version 2. It is not used unless it has been selected with the feature command. Here FEN is a position in Forsythe-Edwards Notation, as defined in the PGN standard.

    Illegal positions: Note that either setboard or edit can be used to send an illegal position to the engine. The user can create any position with xboard's Edit Position command (even, say, an empty board, or a board with 64 white kings and no black ones). If your engine receives a position that it considers illegal, I suggest that you send the response "tellusererror Illegal position", and then respond to any attempted move with "Illegal move" until the next new, edit, or setboard command.

edit
    The edit command is the old way to set up positions. For compatibility with old engines, it is still used by default, but new engines may prefer to use the feature command (see below) to cause xboard to use setboard instead. The edit command puts the chess engine into a special mode, where it accepts the following subcommands:
    c	change current piece color, initially white
    Pa4 (for example)	place pawn of current color on a4
    xa4 (for example)	empty the square a4 (not used by xboard)
    #	clear board
    .	leave edit mode
    See the Idioms section below for additional subcommands used in ChessBase's implementation of the protocol.

    The edit command does not change the side to move. To set up a black-on-move position, xboard uses the following command sequence:

        new
        force
        a2a3
        edit
        <edit commands>
        .

    This sequence is used to avoid the "black" command, which is now considered obsolete and which many engines never did implement as specified in this document.

    After an edit command is complete, if a king and a rook are on their home squares, castling is assumed to be available to them. En passant capture is assumed to be illegal on the current move regardless of the positions of the pawns. The clock for the 50 move rule starts at zero, and for purposes of the draw by repetition rule, no prior positions are deemed to have occurred.
hint
    If the user asks for a hint, xboard sends your engine the command "hint". Your engine should respond with "Hint: xxx", where xxx is a suggested move. If there is no move to suggest, you can ignore the hint command (that is, treat it as a no-op).

bk
    If the user selects "Book" from the xboard menu, xboard will send your engine the command "bk". You can send any text you like as the response, as long as each line begins with a blank space or tab (\t) character, and you send an empty line at the end. The text pops up in a modal information dialog.

undo
    If the user asks to back up one move, xboard will send you the "undo" command. xboard will not send this command without putting you in "force" mode first, so you don't have to worry about what should happen if the user asks to undo a move your engine made. (GNU Chess 4 actually switches to playing the opposite color in this case.)

remove
    If the user asks to retract a move, xboard will send you the "remove" command. It sends this command only when the user is on move. Your engine should undo the last two moves (one for each player) and continue playing the same color.

hard
    Turn on pondering (thinking on the opponent's time, also known as "permanent brain"). xboard will not make any assumption about what your default is for pondering or whether "new" affects this setting.

easy
    Turn off pondering.

post
    Turn on thinking/pondering output. See Thinking Output section.

nopost
    Turn off thinking/pondering output.

analyze
    Enter analyze mode. See Analyze Mode section.

name X
    This command informs the engine of its opponent's name. When the engine is playing on a chess server, xboard obtains the opponent's name from the server. When the engine is playing locally against a human user, xboard obtains the user's login name from the local operating system. When the engine is playing locally against another engine, xboard uses either the other engine's filename or the name that the other engine supplied in the myname option to the feature command. By default, xboard uses the name command only when the engine is playing on a chess server. Beginning in protocol version 2, you can change this with the name option to the feature command; see below.

rating
    In ICS mode, xboard obtains the ICS opponent's rating from the "Creating:" message that appears before each game. (This message may not appear on servers using outdated versions of the FICS code.) In Zippy mode, it sends these ratings on to the chess engine using the "rating" command. The chess engine's own rating comes first, and if either opponent is not rated, his rating is given as 0. In the future this command may also be used in other modes, if ratings are known. Example:

    rating 2600 1500

ics HOSTNAME
    If HOSTNAME is "-", the engine is playing against a local opponent; otherwise, the engine is playing on an Internet Chess Server (ICS) with the given hostname. This command is new in protocol version 2 and is not sent unless the engine has enabled it with the "feature" command. Example: "ics freechess.org"

computer
    The opponent is also a computer chess engine. Some engines alter their playing style when they receive this command.

pause
resume
    (These commands are new in protocol version 2 and will not be sent unless feature pause=1 is set. At this writing, xboard actually does not use the commands at all, but it or other interfaces may use them in the future.) The "pause" command puts the engine into a special state where it does not think, ponder, or otherwise consume significant CPU time. The current thinking or pondering (if any) is suspended and both player's clocks are stopped. The only command that the interface may send to the engine while it is in the paused state is "resume". The paused thinking or pondering (if any) resumes from exactly where it left off, and the clock of the player on move resumes running from where it stopped.
 
 */
}
