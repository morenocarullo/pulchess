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
 testSuiteRook();}

#define PULCHESS_CALLCASE(C,M) { printf("%s start...", M); C(); printf("\t...completed\n"); };
#define assert_true(C) \
 if( !(C) )  {\
  printf("fail on %s line %s\n", suitename, __LINE__);\
  exit(1);\
 }
 
void testSuiteSoldier(void);
void testSuiteKing(void);
void testSuiteRook(void);
