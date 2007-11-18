/*
  PulCHESS, a Computer Chess program
                by Moreno Carullo
 
  About this file:
         XBoard module interface.
 
  License:
         GPL v2, see license.txt in project root.
 
  Version:
         $Id$
*/
#ifndef _pulchess_xboard_h_
#define _pulchess_xboard_h_

#include <string>
#include "pulchess.H"

using namespace std;
using namespace pulchess::logic;

namespace pulchess {

//! An XBoard interface	
class XBoard
{
  private:
    Pulchess * pulchess;
    string buff;
    bool run;
    bool readCommand();   
    void unknownCommand(string &cmd);
    void unimplCommand(string &cmd);
    void badCommand(string &cmd);
    
    //! Ask the engine to play a move
    void goCommand();

    //! Send xboard2 features
    void sendFeatures();

    //! Check if the game is finished and send back info
    void CheckSendResult();
      
  public:
    XBoard();
    ~XBoard();
    
    void mainLoop();
};

}

#endif
