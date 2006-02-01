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
