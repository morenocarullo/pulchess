/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo moreno.carullo [at] pulc [dot] it
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Unit test launch program
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
 * Created on ?
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

