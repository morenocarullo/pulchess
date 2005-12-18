/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Bishop implementation
 *
 * Created on 22-lug-2005
 * $Id$
 */
#include "logic/stdheader.h"

namespace pulchess { namespace logic {

  int Bishop::getKind()
  {
    return PIECE_BISHOP;
  }
  
  int Bishop::getRank()
  {
    return 3; // bishop's rank
  }

  byte Bishop::getValue()
  {
    return (byte)7 + colour*3;
  }

  bool Bishop::isValidMove(byte newpos, Board * b)
  {
    return isValidMove_diag(newpos, b);
  }
  
  list<Move *> * Bishop::listMoves(Board* b)
  {
    // MACRO bishop_add_move(LABEL)
    //
    // - Se la casella di destinazione e' piena, controlla che il colore
    //   della pedina che ci risiede sia del colore opposto. In ogni caso
    //   finisce la ricerca delle mosse
    // - Se la casella di dest. e' vuota, allora la mossa va bene e si puo'
    //   continuare a cercare mosse.
    //
    ///////////////////////////
#define bishop_add_move(GOTOLABEL) {\
        p = b->getPiece(i,j); \
        if( i==x && j==y ) continue; \
	if( p != NULL ) { \
	  if( isEnemy(p) ) { \
	    mList->push_back( new Move( xy2pos(i,j), pos) ); \
	  } \
	  goto GOTOLABEL; \
	} \
	if( p == NULL ) { \
	  mList->push_front( new Move( xy2pos(i,j), pos) ); \
	}\
    }\

    list<Move *> * mList = new list<Move *>();
    Piece * p;
    
    // diagonale 1
    //  x
    //   \
    //    \
    /////////////////////////			    
    for(int i=x+1, j=y+1; i<8 && j<8; i++, j++) {
      bishop_add_move(end_diag_1);
    }

  end_diag_1:

    // diagonale 2
    //  \
    //   \
    //    x
    /////////////////////////			    
    for(int i=x-1, j=y-1; i>=0 && j>=0; i--, j--) {
      bishop_add_move(end_diag_2);
    }

  end_diag_2:

    // diagonale 3
    //     x
    //    /
    //   /
    /////////////////////////			    
    for(int i=x, j=y; i>=0 && j<8; i--, j++) {
      bishop_add_move(end_diag_3);
    }

  end_diag_3:

    // diagonale 3
    //     /
    //    /
    //   x
    /////////////////////////			    
    for(int i=x+1, j=y-1; i<8 && j>=0; i++, j--) {
      bishop_add_move(end_moves);
    }

  end_moves:
    return mList;
  }

};
};
