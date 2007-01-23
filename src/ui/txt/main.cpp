/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE:	   CLI pulchess main file
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
#include "pulchess.H"
#include "xboard.H"
#include <iostream>

using namespace std;
using namespace pulchess;
using namespace pulchess::logic;


//! Print the welcome lines
static void printGreeting(void)
{
  cout << Pulchess::GetPulchessVersion() << " -- " << __DATE__ << endl;
  cout << "2005-2007 (C) Moreno Carullo" << endl;
  cout << "" << endl;
  cout << "Type quit anytime to exit."<< endl;
  cout << "" << endl;  
}

//! Entry point for the CLI version of Pulchess
int main(int argc, char *argv[])
{
  Pulchess * pulchess = NULL;
  string mode, cmd;
  bool menuOk = false;

  printGreeting();

  do
  {
  	cout << "Game mode? (hc, cc, ch, hh, quit) > ";
	  cin >> mode;
	  cout << endl;
  
	  if( mode == "hc"  )
	  {
	    pulchess = new Pulchess(HUM_VS_CPU);
	    menuOk = true;
	    cout << "Human Vs CPU" << endl;
	    cout << endl;
	  }
	  else if( mode == "cc" )
	  {
	    pulchess = new Pulchess(CPU_VS_CPU);
	    menuOk = true;
	    cout << "CPU Vs CPU" << endl;
	    cout << endl; 
	  }
	  else if(mode == "hh")
	  {
	    pulchess = new Pulchess(HUM_VS_HUM);
	    menuOk = true;
	    cout << "Human Vs Human" << endl;
	    cout << endl;
	  }
	  else if( mode == "ch"  )
	  {
	    pulchess = new Pulchess(CPU_VS_HUM);
	    menuOk = true;
	    cout << "CPU Vs Human" << endl;
	    cout << endl;
	  }
	  else if(mode == "xboard")
	  {
	    XBoard * xboard = new XBoard();
	    xboard->mainLoop(); 
	    delete xboard;
	    return 0;
	  }
	  else if(mode == "quit")
	  {
	    return 0;
	  }
	  else
	  {
	     cout << "Command not valid! Please choose another one..." << endl;	
	  }
  }
  while(!menuOk);

  pulchess->Init();
  pulchess->StartGame();

  while( !pulchess->IsGameFinished() ) {
    pulchess->printBoard();
    if( pulchess->IsHuman() )
    {
      cout << "Mossa? > ";
	  cin >> cmd;
	  cout << endl;
	  if( cmd == "quit" ) return 0;
    }
    if( !pulchess->gameCommand(cmd) )
    {
        cout << "Invalid move." << endl;
    }
  }
  
  cout << (pulchess->gameInfo() == WHITE ? "White" : "Black") << " wins." << endl;
  pulchess->printBoard();
}