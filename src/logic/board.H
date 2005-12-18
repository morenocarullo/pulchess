/*
 * PROJECT: PulCHESS, a Computer Chess program
 * LICENSE: GPL, see license.txt in project root
 * FILE: Board and BoardValue interfaces
 *
 * Created on 15-lug-2005
 * $Id$
 */
#include "logic/stdheader.h"

#ifndef _pulchess_logic_board_h_
#define _pulchess_logic_board_h_

#define pos2x(P)    ( P%8 )
#define pos2y(P)    ( P/8 )
#define xy2pos(X,Y) ( (Y) * 8 + (X) )

namespace pulchess { namespace logic {

  class Piece;
  class PlayerIF;

  // Rappresenta la scacchiera
  //
  class Board {
    
  private:
    
    /* properties */
    list<Piece *> whiteList;
    list<Piece *> blackList;
    Piece * _map[64];          
    King * _whiteKing;
    King * _blackKing;
    PlayerIF * _whitePlayer;
    PlayerIF * _blackPlayer;
    PlayerIF * _backupPlayer;
    
    /* methods */
    void _checkCoords(const coord_t x, const coord_t y);      
    void _putPiece(const coord_t x, const coord_t y, Piece* p);

  public:

    Board(PlayerIF * white, PlayerIF * black);
    ~Board();

    // TODO: usare dei metodi di accesso!
    colour_t turn;
    int moveCount;
    
    Piece * getPiece(const coord_t x, const coord_t y);
    Piece * getPiece(const byte idx);
    void    setPiece(const byte pos, Piece *p);
    void    pieceListAdd(Piece *p);
    void    pieceListDel(Piece *p);

    list<Piece *> * listPieces(const colour_t colour);
    PlayerIF * getPlayer(colour_t colour);
    Piece * getKing(const colour_t colour);
    bool isGameFinished();
    int getMoveCount();
    int evaluate();
    int whoWins();
    bool isInCheck(const colour_t colour);
	bool canEatThis(coord_t pos, const colour_t colour);
    bool canDefendCheck(const colour_t colour);
    Move * checkDefenseMove(const colour_t colour);
    void switchAutoThinking(const colour_t colour);

    //
#ifdef DEBUG
    void printBoard();
    void checkBoard();
#endif

  };

  // Rappresenta la scacchiera in modo compatto
  /////////////////////////////////////////////
  class BoardValue {
  private:
    byte map[64];
    byte depth;
    int hashkey;

  public:
    BoardValue(Board *b, byte depth, unsigned int dstTableSize);
    ~BoardValue();

    unsigned int getHashKey();
    byte getDepth();
    bool usableFor(BoardValue *b);
    byte* getMap();
  };

};
};

#endif
