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

namespace pulchess {

//! An XBoard interface	
class XBoard
{
  private:
    bool run;      
          
    bool readCommand(const char * cmd);
    void unknownCommand(const char * cmd);
    void unimplCommand(const char *cmd);
      
  public:
    XBoard();
    ~XBoard();
    
    void mainLoop();
};

}
