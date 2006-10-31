#import "PCMainUiController.h"
#import "PCGameThread.h"

@implementation PCMainUiController

- (void)awakeFromNib
{
	gameFacade  = nil;
	NSLog(@"PC Controller started");
}

- (IBAction)newGame:(id)sender
{
	// se non c'e' gia' un gioco in esecuzione...
	if( gameFacade == nil ) {
		gameFacade  = new pulchess::logic::Pulchess(HUM_VS_CPU);
		gameFacade->init();
		NSLog(@"Pulchess game logic init ok");
		
	    // imposta tutti i pezzi da disegnare
	    [self updateChessBoard];	
		
		[self gameStep:self];
		
		[boardView setNeedsDisplay:YES];
	}
}

// Processa un turno di gioco
// E' chiamata in un thread separato se pe la cpu.
- (void)gameStep:(id)sender
{
	if( !gameFacade->IsGameFinished() ) {
		if( !gameFacade->isHuman() ) {
            string cmd;
			[gameStatus setStringValue:@"Thinking..."];
			gameFacade->gameCommand(cmd);
			[gameStatus setStringValue:@""];
			
			[self updateChessBoard];
		}
		else {
			// turno umano... spara 1 messaggio a video
			NSLog(@"Giocatore: gioca il tuo turno!");
		}
	}
	else {
		NSLog(@"Qualcuno ha vinto...");
	}
}

//
// Aggiorna la scacchiera e le altre informazioni
//
- (void)updateChessBoard
{
	cellinfo_t cellinfo;
	int i=0, j=0;
	
    // scacchiera
	for(i=0;i<8;i++) {
		for(j=0;j<8;j++) {
			cellinfo = gameFacade->getCellInfo(i,j);
			[boardView setPiece:cellinfo.kind col:cellinfo.colour x:i y:j];
		}
	}
	[boardView setNeedsDisplay:YES];
    
    // lista mosse
    NSString *gameReport;
    gameReport = [NSString stringWithCString:gameFacade->GetGameMovesReport().c_str()];
    [gameHistoryView setString:gameReport];
    [gameHistoryView setNeedsDisplay:YES];
}

- (IBAction)acceptMove:(id)sender
{
	// Errore: gioco non ancora inizializzato!
	if( gameFacade == nil )
		return;

	string str = [[moveText stringValue] cString];
	[moveText setStringValue:@""];
	
	// mossa non valida!
	if( !gameFacade->gameCommand(str) ) {
		[gameStatus setStringValue:@"Mossa non valida!"];
		return;
	}
	
	// ok questa mossa e' finita, aggiorniamo la board
	[self updateChessBoard];
    	
	// separiamo in un thread la parte del turno successivo...
	[NSThread detachNewThreadSelector:@selector(DoThinking:) toTarget:[PCGameThread class] withObject:self];
}

- (IBAction)quit:(id)sender
{
	// do destroy stuff here! :)
	if( gameFacade != nil ) {
		delete gameFacade;
	}
	exit(0);
}

@end
