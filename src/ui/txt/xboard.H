/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE:	   XBoard interface - header file
* 
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
*
* Created on 20-giu-2006
* $Id$
*/
#ifndef _pulchess_xboard_h_
#define _pulchess_xboard_h_

#include <string>
#include "pulchess.H"

using namespace std;
using namespace pulchess::logic;

namespace pulchess {

//! An XBoard interface	
class XBoard
{
  private:
    Pulchess * pulchess;
    string buff;
    bool run;
    bool readCommand();   
    void unknownCommand(string &cmd);
    void unimplCommand(string &cmd);
    
    //! Ask the engine to play a move
    void goCommand();

    //! Send xboard2 features
    void sendFeatures();

    //! Check if the game is finished and send back info
    void CheckSendResult();
      
  public:
    XBoard();
    ~XBoard();
    
    void mainLoop();
};

}

#endif
