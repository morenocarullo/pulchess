/*
  PulCHESS, a Computer Chess program
                by Moreno Carullo
 
  About this file:
         Pulchess global definitions.
 
  License:
         GPL v2, see license.txt in project root.
 
  Version:
         $Id$
*/
#ifndef _PULCHESS_LOGIC_STDHEADER_H_
#define _PULCHESS_LOGIC_STDHEADER_H_

#ifndef PULCHESS_NOTABLES
#define PULCHESS_USEHASHTABLE
#endif

#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>

typedef signed char coord_t;
typedef unsigned char byte;
typedef signed char colour_t ;

extern bool pulchess_log_on;
extern bool pulchess_debug;

#define WHITE 1
#define BLACK -1
#define WHITE_WINS    1000000
#define BLACK_INCHECK  100000
#define BLACK_WINS   -1000000
#define WHITE_INCHECK -100000
#define CHECK_PLUS		1000

#define NO_POSITION  65

#define pos2x(P)    ( P%8 )
#define pos2y(P)    ( P/8 )
#define xy2pos(X,Y) ( (Y) * 8 + (X) )
#define abs(A) ( (A)<0 ? -(A) : (A) )
#define MAX(A,B) ((A)>(B) ? (A) : (B))
#define ENEMY(C) ( C == BLACK ? WHITE : BLACK )
#define ISRELCELL(X) ( colour == BLACK ? (getY() == 8-(X)) : (getY() == (X)-1) )
#define COORDSOK(X,Y) ( (X) >=0 && (X) <8 && (Y) >= 0 && (Y) < 8 )
#define OKCOORDS(I)   ( (I) >=0 && (I) <64 )

#define pulchess_board     Board::board
#define pulchess_the_white Player::whitePlayer
#define pulchess_the_black Player::blackPlayer

#define pulchess_log_on() { pulchess_log_on = true; }
#define pulchess_log_off() { pulchess_log_on = false; }
#define pulchess_log(S)    { if(pulchess_log_on) cerr << "[info] " << S << endl; }
#define pulchess_info(S)   { if(pulchess_log_on) cerr << "[info] " << S << endl; }
#define pulchess_error(S)  { if(pulchess_log_on) cerr << "[error] " << S << endl; }
#ifdef DEBUG
#define pulchess_debug(S) { if(pulchess_log_on) cerr << "[debug] " << S << endl; }
#else
#define pulchess_debug(S) ;
#endif

#include "piece.H"
#include "board.H"
#include "move.H"
#include "cpuplayer.H"
#include "humanplayer.H"
#include "player.H"
#include "hashcache.H"

#endif //_PULCHESS_LOGIC_STDHEADER_H_
