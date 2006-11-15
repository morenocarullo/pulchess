/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE:	   XBoard interface    ***** WORK IN PROGRESS ******
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
	cout.setf(ios::unitbuf);
}

XBoard::~XBoard()
{
}

//
// Handle the "unknown" message to user and to xboard
//
void XBoard::unknownCommand(string &cmd)
{
}

//
// Handle the "not supported" message to user and to xboard
//
void XBoard::unimplCommand(string &cmd)
{
	cerr << "[pulchess-error] unknown comman " << cmd << endl;
}

bool XBoard::readCommand()
{
  cin >> buff;
  return true;
}

//
// Main game loop
//
void XBoard::mainLoop()
{
  run = true;
  Pulchess * pulchess = NULL;

  while(readCommand() && run)
  {
	// if move... play it
	if( pulchess != NULL && pulchess->IsMove(buff))
    {
	  pulchess->gameCommand(buff);
	  continue;
    }
	
	// start deciding on the first char
    switch(buff[0])
    {
	  // new
	  case 'n':
	    if( buff == "new" )
	    {
	      pulchess = new Pulchess(HUM_VS_CPU);
	    }
	    break;
	
	  // new
	  case 'm':
	    //if( buff == "move" )
	    //{
        //   usa sscanf per trovare la mossa 
        //}
	    break;

	  // variant - UNSUPPORTED
	  case 'v':
	    unimplCommand(buff);
	    break;

	  // quit 
	  case 'q':
	    if( buff == "quit" )
	    {
		  unknownCommand(buff);
		  continue;
	    }
	    run = false;
	    break;

      // gioco randomizzato
	  case 'r':
		if( buff == "random" )
	    {
		  unknownCommand(buff);
		  continue;
	    }
	    unimplCommand(buff);	  
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
      //
      // Leave force mode and set the engine to play the color that is on move.
      // Associate the engine's clock with the color that is on move,
      // the opponent's clock with the color that is not on move.
      // Start the engine's clock. Start thinking and eventually make a move.
      case 'g':
		if( buff == "go" )
        {
		  if( pulchess == NULL ) continue;
		  if( !pulchess->isHuman() )
          {
	        pulchess->gameCommand();
            Move * lastMove = pulchess->GetLastMove();
            cout << "move " << lastMove->toString() << endl;
          }
		  continue;
        }
	    break;

      // white
      //
      // Set White on move. Set the engine to play Black. Stop clocks.
      case 'w':
		if( buff == "white" )
        {
          pulchess->ResetMode(HUM_VS_CPU);
		  continue;
        }
        break;

      // black
      //
      // Set Black on move. Set the engine to play White. Stop clocks.
      case 'b':
		if( buff == "black" )
        {
          pulchess->ResetMode(CPU_VS_HUM);
		  continue;
	    }
        break;

	case 'l':
		break;

	case '?':
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

} // end mainLoop()
} // end ns pulchess