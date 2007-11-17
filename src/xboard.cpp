/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:	XBoard interface
 * NOTES:   Not finished yet!
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
 * Created on 20-giu-2006
 * $Id$
*/
#include "xboard.H"
#include <iostream>
#include <cstdio>

using namespace std;
using namespace pulchess::logic;

namespace pulchess {

XBoard::XBoard()
{
	cout.setf(ios::unitbuf);
	pulchess_log_off();
	sendFeatures();
}

XBoard::~XBoard()
{
}

//
// Send features to xboard
//
void XBoard::sendFeatures()
{
	cout << "feature ";
	cout << "myname=\"" << Pulchess::GetPulchessVersion() <<"\" ";
	cout << "sigint=0 sigterm=0 ";
    cout << "analyze=0";
	cout << endl;
}

//
// Handle the "unknown" message to user and to xboard
//
void XBoard::unknownCommand(string &cmd)
{
	cout << "Error (unknown command): " << cmd << endl;
}

//
// Handle the "not supported" message to user and to xboard
//
void XBoard::unimplCommand(string &cmd)
{
	cout << "telluser unimplemented command!:" << cmd << endl;
}

void XBoard::badCommand(string &cmd)
{
	cout << "telluser bad syntax for command: " << cmd << endl;
}

//
// Read command from standard input
//
bool XBoard::readCommand()
{
  do {
    getline(cin, buff);
  }
  while( buff.length() == 0);

  return true;
}

//
// Go! Command
// --
// Leave force mode and set the engine to play the color that is on move.
// Associate the engine's clock with the color that is on move, the opponent's clock with the color that is not on move.
// Start the engine's clock. Start thinking and eventually make a move.
//
void XBoard::goCommand()
{
  if( pulchess == NULL ) {
	return;
  }

  delete pulchess_the_white;
  delete pulchess_the_black;	

  if( pulchess->whoPlaysNow() == PULCHESS_WHITE ) {
    pulchess_the_white = new CPUPlayer(PULCHESS_WHITE);
    pulchess_the_black = new HumanPlayer(PULCHESS_BLACK);   
  }
  else {
    pulchess_the_white = new HumanPlayer(PULCHESS_WHITE);
    pulchess_the_black = new CPUPlayer(PULCHESS_BLACK);
  }
     
  pulchess->gameCommand();
  Move * lastMove = pulchess->GetLastMove();
  cout << "move " << lastMove->toString() << endl;
  CheckSendResult();
}

//
// Check if game is finished and send back info
//
void XBoard::CheckSendResult()
{
	if( pulchess == NULL ) return;
	if( pulchess->IsGameFinished() )
	{
		string result;
		switch(pulchess->gameInfo())
		{
			case PULCHESS_WHITE:
			  result = "0-1 {Black mates}";
			  break;
			
			case PULCHESS_BLACK:
			  result = "1-0 {White mates}";
			  break;
		}
		cout << result << endl;
	}
}

//
// Main game loop
//
void XBoard::mainLoop()
{
  run = true;
  pulchess = NULL;

  while(readCommand() && run)
  {
	// if it is a move... play it
	if( pulchess != NULL && pulchess->IsMove(buff))
    {
	  if( !pulchess->gameCommand(buff) )
      {
	     cout << "Illegal move: " << buff << endl;	
      }
      else
      {
         CheckSendResult();
      }
	  if( !pulchess->IsGameFinished() )
      {
        goCommand();
	  }
	  continue;
    }
	
	// start deciding on the first char
    switch(buff[0])
    {  
	  case 'n':
	
	    // new
	    if( buff == "new" )
	    {
	      pulchess = new Pulchess(HUM_VS_CPU);
	      pulchess->Init();
          pulchess->StartGame();
	    }
	
	    // nopost
		if( buff == "nopost" )
		{
		  CPUPlayer::xboardPost = false;
		}
	    break;

	  // variant - UNSUPPORTED
	  case 'v':
	    unimplCommand(buff);
	    break;
	
	  // post
	  case 'p':
	    if( buff == "post") {
		  CPUPlayer::xboardPost = true;
        }
		break;

	  // quit 
	  case 'q':
	    if( buff == "quit" )
	    {
	     run = false;
	     delete pulchess;
	     return;
	    }
	    break;

      // gioco randomizzato
	  case 'r':
		if( buff == "random" )
		{
			// randomize plays
			// currently pulchess ignores this
		}	  
	    break;

      // force mode
      //
      // with this the engine should only check move correctness.
      // for 2 human players, I think.
	  case 'f':
		if( buff == "force" )
	    {
		  unknownCommand(buff);
		  continue;
	    }
	    unimplCommand(buff);
	    break;

      // go
      case 'g':
		if( buff == "go" )
        {
          goCommand();
		  continue;
        }
	    break;

      // white
      //
      // Set White on move. Set the engine to play Black. Stop clocks.
      case 'w':
		if( buff == "white" && pulchess != NULL)
        {
          pulchess->ResetMode(HUM_VS_CPU);
		  continue;
        }
        break;

      // black
      //
      // Set Black on move. Set the engine to play White. Stop clocks.
      case 'b':
		if( buff == "black"  && pulchess != NULL)
        {
          pulchess->ResetMode(CPU_VS_HUM);
		  continue;
	    }
        break;

	case 'l':
	    // level
	    // -> type 1 or 2 of time control.
		if( buff.substr(0,5) == "level"  && pulchess != NULL) {
          pulchess_debug("Trying to set the clock...");
	      unsigned int _m=40, _s=5, _i=0;
          if( sscanf(buff.c_str(), "level %d %d %d", &_m, &_s, &_i) == 3 ) {
            pulchess->SetTimecontrol(_m,_s*60,_i);
            continue;
          }
        }
		break;

	case '?':
		break;
		
    default:
      unknownCommand(buff);
      break;
	}
  }

} // end mainLoop()
} // end ns pulchess
