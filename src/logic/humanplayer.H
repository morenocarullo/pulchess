#include "logic/stdheader.h"

#ifndef _pulchess_logic_humanplayer_h_
#define _pulchess_logic_humanplayer_h_

/*
 * Created on 28-gen-2005
 *
 */

namespace pulchess { namespace logic {

  class HumanController;


  /**
   * @author moreno
   */
  class HumanPlayer: public PlayerIF
  {
  private:
    HumanController * controller;
    Move * getMove();
  
  public:
    HumanPlayer(colour_t colour, HumanController * controller);
    ~HumanPlayer();

    void doYourMove();
    Piece * chooseSoldierPiece();
};

};
};

#endif
