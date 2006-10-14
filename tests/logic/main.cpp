/*
 *  main.cpp
 *  pulchess
 *
 *  Created by Moreno Carullo on 15/02/06.
 *  Copyright 2006 Moreno Carullo. All rights reserved.
 *
 * $Id$
 */
#include <iostream>
#include "stdheader.h"
#include "tests.h"

int main(int argc, char *argv[]) {
	pulchess_log_off();
	PULCHESS_RUNTESTS();
	cout << endl << "test completed successfully." << endl;
	return 0;
}

