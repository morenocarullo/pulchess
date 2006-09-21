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
#include "stdheader.h"
#include "tests.h"

int main(int argc, char *argv[]) {
	pulchess_log_off();
	PULCHESS_RUNTESTS();
	printf("\n");
	return 0;
}

