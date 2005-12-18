#ifndef _PULCHESS_LOGIC_STDHEADER_H_
#define _PULCHESS_LOGIC_STDHEADER_H_

#include <string>
#include <list>
#include <vector>
#include <iostream> // DEBUG
#include <cstdio>  // DEBUG

typedef char coord_t;
typedef char byte;
typedef signed char colour_t ;

#define WHITE 1
#define BLACK -1
#define WHITE_WINS    1000000000
#define BLACK_INCHECK  100000000
#define BLACK_WINS   -1000000000
#define WHITE_INCHECK -100000000

#define abs(A) ( (A)<0 ? -(A) : (A) )
#define ENEMY(C) ( C == BLACK ? WHITE : BLACK )
#define ISRELCELL(X) ( getColour() == BLACK ? (getY() == 8-(X)) : (getY() == (X)-1) )
#define COORDSOK(X,Y) ( (X) >=0 && (X) <8 && (Y) >= 0 && (Y) < 8 )
#define OKCOORDS(I)   ( (I) >=0 && (I) <64 )

#define NOT_A_COORD   11

#include "logic/piece.H"
#include "logic/tower.H"
#include "logic/queen.H"
#include "logic/king.H"
#include "logic/knight.H"
#include "logic/soldier.H"
#include "logic/bishop.H"
#include "logic/board.H"
#include "logic/move.H"
#include "logic/cpuplayer.H"
#include "logic/humanplayer.H"
#include "logic/humancontroller.H"
#include "logic/playerif.H"
#include "logic/CoordsException.h"
#include "logic/InvalidMoveException.h"
#include "logic/hashcache.H"



#endif //_PULCHESS_LOGIC_STDHEADER_H_
