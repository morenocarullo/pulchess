/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: CpuPlayer interface
 *
 * Created on 15-lug-2005
 * $Id$
 */
#include "logic/stdheader.h"

#ifndef _pulchess_logic_cpuplayer_h_
#define _pulchess_logic_cpuplayer_h_

#include "logic/playerif.H"
#include "logic/hashcache.H"
#include "logic/timecontrol.H"

namespace pulchess { namespace logic {

  class Move;
  class HashCache;

  class CPUPlayer : public PlayerIF
  {
  private:
    Move *bestMove;
	
	//
    int   plyDeep;
	
	//
    HashCache *evc;
	
	//
	bool  ffprob;

    time_t moveCalcTime;
    TimeControl *timec;
    
  public:
    // class constructor
    CPUPlayer(colour_t colour);
    // class destructor
    ~CPUPlayer();
    void doYourMove();
    int alfabeta(int depth, colour_t turnColour, int alfa, int beta);
    Piece * chooseSoldierPiece();
  };
  
};
};

#endif // CPUPLAYER_H
