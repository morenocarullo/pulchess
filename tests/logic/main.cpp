/*
 *  main.cpp
 *  pulchess
 *
 *  Created by Moreno Carullo on 15/02/06.
 *  Copyright 2006 Moreno Carullo. All rights reserved.
 *
 * $Id$
 */
#include <stdio.h>
#include "tests.h"

int main(int argc, char *argv[]) {
	PULCHESS_RUNTESTS();
	printf("pulchess logic tests completed.\n");
	return 0;
}

