/*
  PulCHESS, a Computer Chess program
                by Moreno Carullo
 
  About this file:
         This file contains the main pulchess facade (implementation),  
         used to expose to other programs the pulchess logic.
 
  License:
         GPL v2, see license.txt in project root.
 
  Version:
         $Id$

*/
#include <sstream>
#include "stdheader.h"
#include "pulchess.H"
#include "book.H"

/** pulchess static local vars */
static const char version[] = "PulCHESS 0.2k";

/** pulchess global vars */
bool pulchess_log_on = true;

/** IS DEBUGGING ON? */
#ifdef DEBUG
bool pulchess_debug = true;
#else
bool pulchess_debug = false;
#endif

namespace pulchess {
	namespace logic {
		
ofstream Pulchess::log;

// Pulchess costructor
Pulchess::Pulchess(gamemode_t gameMode)
{
  this->gameMode = gameMode;
	this->engineStatus = PULCHESS_STATUS_ZERO;
}


// Pulchess destructor
Pulchess::~Pulchess()
{
    Shutdown();
}

//
// Get a string containing engine's name and version
//
string Pulchess::GetPulchessVersion()
{
	return string(version);
}

//
// Change game mode
//
void Pulchess::ResetMode(gamemode_t gameMode)
{
  this->gameMode = gameMode;
  this->engineStatus = PULCHESS_STATUS_ZERO;
  if( Board::board != NULL )
  {
	delete Board::board;
	Board::board = NULL;	 
  }
  Init();
}

//
// Set time control. This has to be called before game starts.
//
//  In conventional clock mode, increment=0.
//  In increment mode, movesToPlay=0, secondsForMoves is the base.
//  In exact seconds for move, secondsForMoves is the only positive number.
//
void Pulchess::SetTimecontrol(int movesToPlay, int secondsForMoves, int increment)
{
  pulchess_debug("Set time control to:" << movesToPlay << "," << secondsForMoves << "," << increment);

  if( increment == 0 && movesToPlay>0 && secondsForMoves > 0) {
   	pulchess_debug("Conventional clock mode: " << movesToPlay << " moves in " << secondsForMoves << " s.");
  }
  else if( movesToPlay == 0 && increment>0 && secondsForMoves > 0) {
    pulchess_debug("Increment clock mode: base " << secondsForMoves << " s and increment " << increment << " s." );
  }
  else if( movesToPlay == 0 && increment == 0 && secondsForMoves > 0) {
    pulchess_debug("Exact seconds clock mode: " << secondsForMoves << " s for move." );
  }

  if( pulchess_the_white != NULL & pulchess_the_black != NULL ) {
    pulchess_the_white->InitClock(movesToPlay, secondsForMoves, increment);
    pulchess_the_black->InitClock(movesToPlay, secondsForMoves, increment);
  }
}

//
// Init engine from a given position
//
void Pulchess::Init()
{
  Init((string)Board::STARTING_FEN);
}

//
// Init engine
//
void Pulchess::Init(string sFenStartPosition)
{
  const int DEFAULT_MAXPLY = 6;

  // seleziona la modalita' di gioco.
  switch( gameMode )
	{
		// computer vs computer
		case CPU_VS_CPU:
			pulchess_the_white = new CPUPlayer(WHITE, DEFAULT_MAXPLY, 30, true);
			pulchess_the_black = new CPUPlayer(BLACK, DEFAULT_MAXPLY, 30, true);
			break;
	
		// umano vs computer
		case HUM_VS_CPU:
			pulchess_the_white = new HumanPlayer(WHITE);
			pulchess_the_black = new CPUPlayer(BLACK, DEFAULT_MAXPLY, 30, true);
			break;
			
		// umano vs computer
		case CPU_VS_HUM:
			pulchess_the_white = new CPUPlayer(WHITE, DEFAULT_MAXPLY, 30, true);
			pulchess_the_black = new HumanPlayer(BLACK);
			break;
			
		// umano vs umano
		case HUM_VS_HUM:
			pulchess_the_white = new HumanPlayer(WHITE);
			pulchess_the_black = new HumanPlayer(BLACK);
			break;
  }
	
  Board::board = new Board(sFenStartPosition);
	engineStatus = PULCHESS_STATUS_INIT;
	Book::Load();
	
	// Default TC mode
	SetTimecontrol(40,300,0);
	
	pulchess_debug("pulchess is in DEBUG mode!");
#ifdef PULCHESS_USEHASHTABLE
	pulchess_debug("pulchess is using hashtables.");
#endif
}

// carica un gioco
bool Pulchess::LoadGame(const char *gamePath)
{
	// crea una classe "controller" che carica le mosse da file
	bool endLoading = false;
	
	FILE *fp  = fopen(gamePath, "rt");
	if( fp == NULL )
	{
		pulchess_error("error opening game file: " << gamePath);
		return false;
	}
	
	pulchess_the_white  = new HumanPlayer(WHITE);
	pulchess_the_black  = new HumanPlayer(BLACK);
  pulchess_board      = new Board();
  engineStatus        = PULCHESS_STATUS_INIT;							
										  
  // TODO: read from file game mode
  //       read from file timings
	
	while(!feof(fp) && !endLoading)
	{
		string mossa;
		char buff[6];
		fscanf(fp, "%s", buff);
		mossa = buff;
		
		if(!gameCommand(mossa))
		{
		  pulchess_error("error loading game file: " << gamePath << " at char " << ftell(fp));
		  fclose(fp);
		  return false;
		}
	}
	
	fclose(fp);
	return true;
}

//
// Start game
//
void Pulchess::StartGame()
{
	if( engineStatus & PULCHESS_STATUS_INIT )
    {
      pulchess_the_white->PushClock();
      engineStatus |= PULCHESS_STATUS_START;
    }
}

//
// Close game
//
void Pulchess::Shutdown()
{
	log.close();
    delete pulchess_board;
    pulchess_board = NULL;
}

//
// Ricostruisce la scacchiera
//
cellinfo_t Pulchess::getCellInfo(int x, int y)
{
    Piece * p;
    cellinfo_t c;
	
    p        = pulchess_board->GetPiece(x,y);
	
    if( p != NULL ) {
		c.kind   = p->getKindChr();
		c.colour = p->getColourChr();
    } else {
		c.kind   = '-';
		c.colour = 0;
    }
	
    return c;
}

//
// Is game ended?
//
bool Pulchess::IsGameFinished()
{
    return (pulchess_board != NULL) ? pulchess_board->IsGameFinished() : true;
}

//
// Who has to move now?
//
int Pulchess::whoPlaysNow()
{
    return (pulchess_board != NULL)  ? pulchess_board->turn : PULCHESS_WHITE;
}

//
// Accept a move command from current player
//
bool Pulchess::gameCommand(string &cmd)
{	
	bool retval;
	
	if( ! (engineStatus & PULCHESS_STATUS_INIT) )
    {
      pulchess_error("pulchess:gameCommand called before init");
      return false;
    }
	
    if( pulchess_board->turn == WHITE ) {
		retval = pulchess_the_white->DoMove(cmd);
    }
    else {
		retval = pulchess_the_black->DoMove(cmd);
    }

    string logMsg("Move command (only for humans): ");
    string messaggio;
    string accepted( retval ? " (accepted)" : " (refused)" );
    messaggio = logMsg + cmd + accepted;
    WriteLog(messaggio);
	
	return retval;
}

//
// Play an empty move
//
bool Pulchess::gameCommand()
{
  string cmd = "";
  return gameCommand(cmd);
}

// La stringa data e' una mossa?
// da spostare nella classe Move come membro statico
bool Pulchess::IsMove(string &cmd)
{
  if( cmd == "O-O" )   return true;
  if( cmd == "O-O-O" ) return true;
  if( cmd[0] <= 'h' && cmd[0] >= 'a' &&
      cmd[2] <= 'h' && cmd[2] >= 'a' && 
      cmd[1] <= '8' && cmd[1] >= '1' &&
      cmd[3] <= '8' && cmd[3] >= '1' )
  {
    return true;	
  }

  return false;
}

// Il prossimo giocatore e' umano?
bool Pulchess::IsHuman()
{
  if( engineStatus == PULCHESS_STATUS_ZERO ) {
	return false;
  }
  if( pulchess_board->turn == WHITE ) {
    return pulchess_the_white->IsHuman();
  }
  else {
    return pulchess_the_black->IsHuman();
  }
}

//
// Is the specified player human?
//
bool Pulchess::IsHuman(const colour_t colour)
{
	if( engineStatus == PULCHESS_STATUS_ZERO ) return false;
	return (colour == WHITE) ? pulchess_the_white->IsHuman() : pulchess_the_black->IsHuman(); 
}

//
// Gimme last played move
//
Move * Pulchess::GetLastMove()
{
	return (pulchess_board == NULL) ? NULL : pulchess_board->GetLastMove();
}

//
// Game's move report
//
string  Pulchess::GetGameMovesReport()
{
  string report = "";
  list<Move *> * mlist;
  list<Move *>::iterator it;
     int ct;	

  if( pulchess_board == NULL ) return report;

  mlist = pulchess_board->GetMoveList();

  for (ct = 0, it = mlist->begin(); it != mlist->end(); it++, ct++)
  {
    if( ct%2 == 0)
    {
	  stringstream strs;
	  string       str;
	  strs << (ct/2+1);
	  strs >> str;
	  report += str;
      report += ". ";
    }
    report += (*it)->toString();
    report += " ";
  }
  return report;
}

//
// Game status
//
int Pulchess::gameInfo()
{
    return pulchess_board->WhoWins();
}

//
// Print board.
// TODO: refactor this into Board
//
void Pulchess::printBoard()
{
  if( pulchess_board != NULL ) {
    pulchess_board->Print();
  }
}

//
// Compute the Perft test
// The perft is the number of generated nodes.
//
int Pulchess::Perft(int depth)
{
  /* Short list of perft values
      1: 20
      2: 400
      3: 8902
      4: 197281
      5: 4865609
  */
  vector<Move *> mList;
  vector<Move *>::iterator mList_iter;
  Move *currMove = NULL;
  int computedMoves = 0;
  
  //
  // Leaf node
  if( depth == 0 )
  {
    return 1;
  }
  
  //
  // Generate moves
  pulchess_board->GenerateMoves(mList, true);

  //
  // Play moves and count them
  for(mList_iter = mList.begin(); mList_iter != mList.end(); mList_iter++)
  {
    currMove = (*mList_iter);
    currMove->Play();
    computedMoves += Perft( depth - 1 );
    currMove->Rewind();
  }
  
  moveListDestroy(&mList);

  return computedMoves;
}

//
// Compute the "divide" task for the perft test
//
void Pulchess::PerftDivide(int nDepth)
{
  vector<Move *> mList;
  vector<Move *>::iterator mList_iter;
  Move *currMove = NULL;
  int computedMoves = 0;
  pulchess_board->GenerateMoves(mList, true);
  
  for(mList_iter = mList.begin(); mList_iter != mList.end(); mList_iter++)
  {
    int subNodesCount;
    currMove = (*mList_iter);
    currMove->Play();
    computedMoves += (subNodesCount = Perft( nDepth - 1 ));
    currMove->Rewind();
    cout << currMove->toString() << "\t" << subNodesCount << endl;
  }
  
  cout << "Total computed moves: " << computedMoves << endl;
  
  moveListDestroy(&mList);
}

//
// Print on standard output the list of generated moves.
//
void Pulchess::PrintMovesForPosition(string fenPosition)
{
  pulchess_info("You requested position: " << fenPosition);
  Board::board = new Board(fenPosition);
  
  vector<Move *> mList;
  vector<Move *>::iterator mList_iter;
  Move *currMove = NULL;

  pulchess_board->GenerateMoves(mList, true);
  
  for(mList_iter = mList.begin(); mList_iter != mList.end(); mList_iter++)
  {
    currMove = (*mList_iter);
    cout << currMove->toString() << endl;
  }
  
  moveListDestroy(&mList);
}

//
// Pulchess logging facility: open log
//
void Pulchess::OpenLog(const char *logFile)
{
  if( logFile == NULL ) return;
  
  log.open(logFile, ios_base::out | ios_base::trunc);
  if( log.is_open() ) {
    log << Pulchess::GetPulchessVersion() << " - started. "<< endl;
  }
}

void Pulchess::CloseLog()
{
  if( log.is_open() ) {
    log.close();
  }
}

//	
// Write to pulchess game log
//
void Pulchess::WriteLog(string &message)
{
  if( log.is_open() ) {
    log << message << endl;
  }
}

//
// Read a line from std input
//
string Pulchess::StdinReadLine()
{
  string buff;
  do {
    getline(cin, buff);
  }
  while( buff.length() == 0);
  return buff;
}

};
};