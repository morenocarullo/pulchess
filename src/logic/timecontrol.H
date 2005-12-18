#ifndef _pulchess_logic_timecontrol_h_
#define _pulchess_logic_timecontrol_h_

#include <time.h>

namespace pulchess { namespace logic {

  class TimeControl {
  private:
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
