/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo
 * LICENSE: GPL, see license.txt in project root
 * FILE:    HumanPlayer header
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
 * Created on 15-lug-2005
 * $Id$
 */
#include "stdheader.h"

#ifndef _pulchess_logic_humanplayer_h_
#define _pulchess_logic_humanplayer_h_

namespace pulchess { namespace logic {

  //! A human player
  class HumanPlayer: public Player
  {
    private:
      //! Used to store the next wanted promotion, specified by user
      int nextPromotedPiece;

      //! Given a move command, which is the required promotion?
      int getPromotion(string moveCmd);

      //! Get the Move out of a move command
      Move * getMove(string moveCmd);
    
    public:
      HumanPlayer(colour_t colour);
      ~HumanPlayer();
		
      bool DoMove(string moveCmd);
      bool IsHuman();
      Piece * ChoosePawnPiece();
	};
	
}; // end ns logic
}; // end ns pulchess

#endif