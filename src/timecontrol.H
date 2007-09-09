/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
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

  /**! Class used to control thinking duration.

  This is NOT used to implement o represent "chess clocks", that is
  handled in the Player class.

  */
  class TimeControl
  {
  private:

    //! When was the timer started?
	time_t startTime;
	
    //! Actual computed `death time'
    time_t deathTime;

    //! The specified time during a measurement. If it stops, it's 0
    time_t secToLive;

    //! Last (real) time took to complete
    time_t lastRealTime;

    //! Last desired secToLive specified by startTimer()
    time_t lastSecToLive;

  public:
    TimeControl();
    ~TimeControl();

    //! Start the time specifying a desired duration
    void startTimer(time_t secToLive);
 
    //! Stop the timer and get it ready for another measurement
    void resetTimer();

    //! Eval the remaining time and return true if we have to hurry up
    bool evalTimeRemaining(unsigned int depth);

	unsigned int GetThinkingTime();

    //! Get the requested time between startTimer() and resetTimer()
    time_t getRequestedTime();

    //! Get total time took between startTimer() and resetTimer()
    time_t getRealTime();

  }; // end class

}; // end logic namespace
}; // end pulchess namespace

#endif // _pulchess_logic_timecontrol_h_