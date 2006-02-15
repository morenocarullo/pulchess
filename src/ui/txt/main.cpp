#include "facade.H"
#include <iostream>
#include <cstdio>
#include <ctype.h> // toupper()

using namespace std;
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

  cout << "Modalita' di gioco? (cpu, hum) > ";
  cin >> mode;
  cout << endl;
  
  if( mode == "cpu"  ) {
    facade = new Facade(HUM_VS_CPU);
    cout << "Umano Vs Comp" << endl;
    cout << endl;
  }
  else {
    facade = new Facade(HUM_VS_HUM);
    cout << "Umano Vs Umano" << endl;
    cout << endl;
  }
  
  facade->setController(new StdInController(), PULCHESS_WHITE);
  facade->setController(new StdInController(), PULCHESS_BLACK);
  facade->init();

  while( !facade->isGameFinished() ) {
    printBoard( facade );
    facade->requestPlay();
  }
}
