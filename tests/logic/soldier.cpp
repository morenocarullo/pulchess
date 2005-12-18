#include "logic/stdheader.h"

#include <iostream>

using namespace pulchess::logic;
using namespace std;

int main(int argc, char **argv) {

  Board b(new CPUPlayer(WHITE), new CPUPlayer(BLACK));
  Soldier * s;
  Knight * n;
  Move * m;

  b.printBoard();
  s = (Soldier *)b.getPiece(0,1);

  list<Move> * mList = s->listMoves(&b);
  list<Move>::iterator lmit;

  for(lmit = mList->begin();  lmit != mList->end();  lmit++){
    lmit->play( &b );
    b.printBoard();
    b.checkBoard();
    lmit->rewind( &b );
    b.printBoard();
    b.checkBoard();
  }

  b.printBoard();
  b.checkBoard();

  return 0;
}
