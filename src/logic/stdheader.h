/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE:    pulchess::logic "module" global header
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
* Created on 15-lug-2005
* $Id$
*/
#ifndef _PULCHESS_LOGIC_STDHEADER_H_
#define _PULCHESS_LOGIC_STDHEADER_H_

#ifndef PULCHESS_NOTABLES
#define PULCHESS_USEHASHTABLE
#endif

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <cstdio>

typedef signed char coord_t;
typedef unsigned char byte;
typedef signed char colour_t ;

extern bool pulchess_log_on;

#define WHITE 1
#define BLACK -1
#define WHITE_WINS    1000000
#define BLACK_INCHECK  100000
#define BLACK_WINS   -1000000
#define WHITE_INCHECK -100000
#define CHECK_PLUS		1000

#define pos2x(P)    ( P%8 )
#define pos2y(P)    ( P/8 )
#define xy2pos(X,Y) ( (Y) * 8 + (X) )
#define abs(A) ( (A)<0 ? -(A) : (A) )
#define ENEMY(C) ( C == BLACK ? WHITE : BLACK )
#define ISRELCELL(X) ( GetColour() == BLACK ? (getY() == 8-(X)) : (getY() == (X)-1) )
#define COORDSOK(X,Y) ( (X) >=0 && (X) <8 && (Y) >= 0 && (Y) < 8 )
#define OKCOORDS(I)   ( (I) >=0 && (I) <64 )

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
#include "rook.H"
#include "queen.H"
#include "king.H"
#include "knight.H"
#include "pawn.H"
#include "bishop.H"
#include "board.H"
#include "move.H"
#include "cpuplayer.H"
#include "humanplayer.H"
#include "playerif.H"
#include "hashcache.H"

#endif //_PULCHESS_LOGIC_STDHEADER_H_
