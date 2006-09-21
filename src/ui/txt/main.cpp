#include "pulchess.H"
#include "xboard.H"
#include <iostream>
#include <cstdio>
#include <ctype.h> // toupper()

using namespace std;
using namespace pulchess;
using namespace pulchess::logic;

class StdInController : public HumanControllerPulchess
{
public:
  string getMove();
  char getPawnPiece();
};

string StdInController::getMove()
{
  string mossa;
  cout << "Mossa? > ";
  cin >> mossa;
  cout << endl;
  return mossa;
}

char StdInController::getPawnPiece()
{
  char soldierpiece;
  cout << "Pezzo? > " << endl;
  cin >> soldierpiece;
  return soldierpiece;
}


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
  string mode;

  printGreeting();

  cout << "Modalita' di gioco? (ucpu, ccpu, hum, xboard) > ";
  cin >> mode;
  cout << endl;
  
  if( mode == "ucpu"  ) {
    facade = new Pulchess(HUM_VS_CPU);
    cout << "Umano Vs Comp" << endl;
    cout << endl;
  }
  else if( mode == "ccpu" ) {
    facade = new Pulchess(CPU_VS_CPU);
    cout << "CPU Vs CPU" << endl;
    cout << endl; 
  }
  else if(mode == "hum") {
    facade = new Pulchess(HUM_VS_HUM);
    cout << "Umano Vs Umano" << endl;
    cout << endl;
  }
  else if(mode == "xboard") {
    XBoard * xboard = new XBoard();
    xboard->mainLoop(); 
    delete xboard;
    return 0;
  }
  else {
    return 1;
  }
  
  facade->setController(new StdInController(), PULCHESS_WHITE);
  facade->setController(new StdInController(), PULCHESS_BLACK);
  facade->init();

  while( !facade->isGameFinished() ) {
    facade->printBoard();
    if( !facade->requestPlay() )
    {
        cout << "Invalid move." << endl;
    }
  }
  
  cout << (facade->gameInfo() == WHITE ? "White" : "Black") << " wins." << endl;
  facade->printBoard();
}
