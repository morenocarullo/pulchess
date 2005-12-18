/*
 * Created on 27-gen-2005
 * 
 * $Id$
 */
using namespace std;

namespace pulchess { namespace logic {

/**
 * @author moreno
 */
class InvalidMoveException {
 private:
  string msg;
  
 public:
  InvalidMoveException() { msg = "-- nessun messaggio specificato --";  }
  InvalidMoveException(string msg) { this->msg = msg; }
  string getMsg() { return msg; }
};

};
};
