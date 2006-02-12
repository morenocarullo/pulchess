/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE:    Time constraint implementation
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
* Created on 31-aug-2005
* $Id$
*/
#include "timecontrol.H"

namespace pulchess { namespace logic {
  
  TimeControl::TimeControl()
  {
    lastRealTime = 0;
    secToLive = 0;
  }
  
  TimeControl::~TimeControl()
  {
  }

  void TimeControl::startTimer(time_t secToLive)
  {
    this->deathTime = time(NULL) + secToLive;
    this->secToLive = secToLive;
  }

  /**
   * 
   */
  void TimeControl::resetTimer()
  {
    lastRealTime = deathTime + secToLive - time(NULL);
    lastSecToLive = secToLive;

    deathTime = 0;
    secToLive = 0;
  }

  time_t TimeControl::getRequestedTime()
  {
    return lastSecToLive;
  }

  time_t TimeControl::getRealTime()
  {
    return lastRealTime;
  }

  bool TimeControl::evalTimeRemaining(unsigned int depth)
  {
    int divver = 100/(depth+1);

    if( (deathTime - time(NULL)) < ( secToLive/divver) )
      return true;
    else
      return false;
  }
  

};};
