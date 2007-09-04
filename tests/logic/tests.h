/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo moreno.carullo [at] pulc [dot] it
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Simple C/C++ unit test header
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
 * Created on 15/02/2006
 * $Id$
 */
#include <stdio.h>

#define PULCHESS_CALLCASE(C,M) { printf("O"); C(); fflush(stdout); };
#define assert_true(C) \
 if( (C) ) printf("."); \
 else  { printf("\n\n!!! UNIT TEST ERROR !!! fail on ´%s´ test suite file %s line %d\n", suitename, __FILE__, __LINE__); exit(1); }
 
void testSuitePawn(void);
void testSuiteKing(void);
void testSuiteRook(void);
void testSuitePulchessPulchess(void);
void testSuiteHumanPlayer(void);
void testSuiteBoard(void);
void testSuiteMove(void);
