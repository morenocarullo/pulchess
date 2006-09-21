/*
 *  tests.h
 *  pulchess
 *
 *  Created by Moreno Carullo on 15/02/06.
 *
 */
#include <stdio.h>
 
#define PULCHESS_RUNTESTS() {\
 testSuiteSoldier();\
 testSuiteKing();\
 testSuiteRook();\
 testSuitePulchessFacade();}

#define PULCHESS_CALLCASE(C,M) { printf("O"); C(); fflush(stdout); };
#define assert_true(C) \
 if( (C) ) printf("."); \
 else  { printf("\n\n!!! UNIT TEST ERROR !!! fail on ´%s´ test suite file %s line %d\n", suitename, __FILE__, __LINE__); exit(1); }
 
void testSuiteSoldier(void);
void testSuiteKing(void);
void testSuiteRook(void);
void testSuitePulchessFacade(void);
