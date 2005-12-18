#include "logic/stdheader.h"

#ifndef _pulchess_logic_humancontroller_h
#define _pulchess_logic_humancontroller_h

namespace pulchess { namespace logic {

  class CoordsMove;

  class HumanController {
  public:
    virtual CoordsMove * getCoordsMove() = 0;
    virtual Piece * getSoldierPiece() = 0;
  };

};
};

#endif
