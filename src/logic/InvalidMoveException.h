/*
 * Created on 27-gen-2005
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
 * $Id$
 */
using namespace std;

namespace pulchess { namespace logic {

/**
 * @author moreno
 */
class InvalidMoveException {
 private:
  string msg;
  
 public:
  InvalidMoveException() { msg = "-- nessun messaggio specificato --";  }
  InvalidMoveException(string msg) { this->msg = msg; }
  string getMsg() { return msg; }
};

};
};
