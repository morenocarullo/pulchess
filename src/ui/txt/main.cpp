#include "pulchess.H"
#include "xboard.H"
#include <iostream>
#include <cstdio>
#include <ctype.h> // toupper()

using namespace std;
using namespace pulchess;
using namespace pulchess::logic;


//
void printGreeting(void)
{
  cout << "Pulchess 0.1 -- " << __DATE__ << endl;
  cout << "" << endl;
}


//
int main(int argc, char *argv[])
{
  Pulchess * facade;
  string mode, cmd;

  printGreeting();

  cout << "Modalita' di gioco? (uc, ccp, hh, x) > ";
  cin >> mode;
  cout << endl;
  
  if( mode == "uc"  ) {
    facade = new Pulchess(HUM_VS_CPU);
    cout << "Umano Vs Comp" << endl;
    cout << endl;
  }
  else if( mode == "cc" ) {
    facade = new Pulchess(CPU_VS_CPU);
    cout << "CPU Vs CPU" << endl;
    cout << endl; 
  }
  else if(mode == "hh") {
    facade = new Pulchess(HUM_VS_HUM);
    cout << "Umano Vs Umano" << endl;
    cout << endl;
  }
  else if(mode == "x") {
    XBoard * xboard = new XBoard();
    xboard->mainLoop(); 
    delete xboard;
    return 0;
  }
  else {
    return 1;
  }
  
  facade->init();

  while( !facade->isGameFinished() ) {
    facade->printBoard();
    if( facade->isHuman() )
    {
      cout << "Mossa? > ";
	  cin >> cmd;
	  cout << endl;
	  if( cmd == "quit" ) return 0;
    }
    if( !facade->gameCommand(cmd) )
    {
        cout << "Invalid move." << endl;
    }
  }
  
  cout << (facade->gameInfo() == WHITE ? "White" : "Black") << " wins." << endl;
  facade->printBoard();
}
