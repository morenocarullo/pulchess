#include "facade.H"
#include "xboard.H"
#include <iostream>
#include <cstdio>
#include <ctype.h> // toupper()

using namespace std;
using namespace pulchess;
using namespace pulchess::logic;

class StdInController : public HumanControllerFacade
{
public:
  string getMove();
  char getSoldierPiece();
};

string StdInController::getMove()
{
  string mossa;
  cout << "Mossa? > ";
  cin >> mossa;
  cout << endl;
  return mossa;
}

char StdInController::getSoldierPiece()
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
void printBoard(Facade * f)
{
  cout << "" << endl;
  for(int y=7; y>=0; y--) {
    printf("%d   ", y+1);
    for(int x=0; x<8; x++) {
      cellinfo_t c = f->getCellInfo(x,y);
      if( c.colour == 'w') {
	cout << (char)toupper(c.kind);
      }
      else {
	cout << c.kind;
      }
      cout << " ";
    }
    cout << endl;
  }

  // intestazione delle colonne
  cout << endl << "    ";
  for(int x=0; x<8; x++) {
    printf("%c ", 'a'+x);
  }
  cout << "" << endl << endl;
}

//
int main(int argc, char *argv[])
{
  Facade * facade;
  string mode;

  printGreeting();

  cout << "Modalita' di gioco? (ucpu, ccpu, hum, xboard) > ";
  cin >> mode;
  cout << endl;
  
  if( mode == "ucpu"  ) {
    facade = new Facade(HUM_VS_CPU);
    cout << "Umano Vs Comp" << endl;
    cout << endl;
  }
  else if( mode == "ccpu" ) {
    facade = new Facade(CPU_VS_CPU);
    cout << "CPU Vs CPU" << endl;
    cout << endl; 
  }
  else if(mode == "hum") {
    facade = new Facade(HUM_VS_HUM);
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
    printBoard( facade );
    if( !facade->requestPlay() )
    {
        cout << "Invalid move." << endl;
    }
  }
  
  cout << "(Someone) wins." << endl;
  printBoard( facade );
}
