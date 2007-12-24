/*
  PulCHESS, a Computer Chess program
                by Moreno Carullo
 
  About this file:
         Command-line pulchess main file.
 
  License:
         GPL v2, see license.txt in project root.
 
  Version:
         $Id$

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
  Pulchess *pulchess = NULL;
  string mode, cmd;
  bool menuOk = false;
  int i;
  const char *logFile = NULL;

  //
  // Parameter assignment
  for(i=0; i<argc; i++) {
    if(strcmp(argv[i], "-log") == 0 ) {
      logFile = argv[i+1];
    }
  }

  //
  // Program starts
  printGreeting();
  Pulchess::OpenLog(logFile);

  do {
  	cout << "Game mode? (hc, cc, ch, hh, quit) > ";
    cin >> mode;
    cout << endl;
  
    // Human Vs CPU: Human as white
    if( mode == "hc"  ) {
      pulchess = new Pulchess(HUM_VS_CPU);
      menuOk = true;
      cout << "Human Vs CPU" << endl;
      cout << endl;
    }
    // CPU vs CPU
    else if( mode == "cc" ) {
      pulchess = new Pulchess(CPU_VS_CPU);
      menuOk = true;
      cout << "CPU Vs CPU" << endl;
      cout << endl; 
    }
    // Human vs Human
    else if(mode == "hh") {
      pulchess = new Pulchess(HUM_VS_HUM);
      menuOk = true;
      cout << "Human Vs Human" << endl;
      cout << endl;
    }
    // Cpu Vs Human: Human as black
    else if( mode == "ch"  ) {
      pulchess = new Pulchess(CPU_VS_HUM);
      menuOk = true;
      cout << "CPU Vs Human" << endl;
      cout << endl;
    }
    // Xboard mode
    else if(mode == "xboard") {
      XBoard * xboard = new XBoard();
      xboard->mainLoop();
      Pulchess::CloseLog();
      delete xboard;
      return 0;
    }
    // Perft test
    else if(mode == "perft") {
      pulchess = new Pulchess(CPU_VS_CPU);
      pulchess->Init();
      int depth;
      cout << "Perft depth = ? ";
      cin >> depth;
      cout << endl;
      pulchess_info("Perft(" << depth << ") = " << pulchess->Perft(depth) );
      delete pulchess;
    }
    // See moves for fen
    else if(mode == "seemoves") {
      string sFenPosition;
      cout << "See moves for FEN position: ";
      cin >> sFenPosition;
      cout << endl;
      pulchess = new Pulchess(CPU_VS_CPU);
      pulchess->Init();
      pulchess->PrintMovesForPosition(sFenPosition);
      delete pulchess;
    }
    // Quit
    else if(mode == "quit") {
      return 0;
    }
    else {
      cout << "Command not valid! Please choose another one..." << endl;	
    }
  }
  while(!menuOk);

  pulchess->Init();
  pulchess->StartGame();

  while( !pulchess->IsGameFinished() ) {
    pulchess->printBoard();
    cmd = "";
    if( pulchess->IsHuman() ) {
      cout << "Mossa? > ";
	  cin >> cmd;
	  cout << endl;
	  if( cmd == "quit" ) return 0;
    }
    if( !pulchess->gameCommand(cmd) ) {
        cout << "Invalid move." << endl;
    }
  }
  
  cout << (pulchess->gameInfo() == WHITE ? "White" : "Black") << " wins." << endl;
  pulchess->printBoard();
  Pulchess::CloseLog();
}