#ifndef _pulchess_logic_facade_h_
#define _pulchess_logic_facade_h_

#include <string>

#define CPU_VS_CPU 0
#define HUM_VS_CPU 1
#define HUM_VS_HUM 2

#define PULCHESS_WHITE -1
#define PULCHESS_BLACK 1

typedef char gamemode_t;
typedef struct {
  char kind;
  char colour;
} cellinfo_t;

using namespace std;

namespace pulchess { namespace logic {

  // human controller facade
  /////////////////////////////
  class HumanControllerFacade
  {
  public:
    virtual string getMove() = 0;
    virtual char getSoldierPiece() = 0;
  };


  // main Pulchess Facade Class
  /////////////////////////////
  class Facade
  {

  private:
    void * board;
    int    turn;
    gamemode_t gameMode;
    HumanControllerFacade * whiteController;
    HumanControllerFacade * blackController;
    void * whitePlayer;
    void * blackPlayer;

  public:

    Facade(gamemode_t gameMode);
    ~Facade();

    // imposta i controller
    void setController(HumanControllerFacade * c, int colour);
    
    // inizializza il gioco
    void init();

    // chiude il gioco
    void shutdown();
    
    // ricostruisce la scacchiera
    cellinfo_t getCellInfo(int x, int y);

    // il gioco e' finito?
    bool isGameFinished();

    // a chi tocca?
    int whoPlaysNow();

    // gioca la mossa del turno attuale
    void requestPlay();

    // chi vince?
    int gameInfo();
  };

};
};

#endif
