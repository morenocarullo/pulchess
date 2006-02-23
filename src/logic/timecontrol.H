/*
* PROJECT: PulCHESS, a Computer Chess program
* LICENSE: GPL, see license.txt in project root
* FILE:    Time constraint interface
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
#ifndef _pulchess_logic_timecontrol_h_
#define _pulchess_logic_timecontrol_h_

#include <time.h>

namespace pulchess { namespace logic {

  class TimeControl {
  private:
	time_t startTime;
    time_t deathTime;
    time_t secToLive;
    time_t lastRealTime;
    time_t lastSecToLive;

  public:
    TimeControl();
    ~TimeControl();

    void startTimer(time_t secToLive);
    void resetTimer();
    bool evalTimeRemaining(unsigned int depth);
    time_t getRequestedTime();
    time_t getRealTime();
  };

};};

#endif
