/* PCMainUiController */

#import <Cocoa/Cocoa.h>
#import "PCChessView.h"
#import "PCGameWrapper.h"
#import "../../src/logic/pulchess.H"

@interface PCMainUiController : NSObject
{
	// view
    IBOutlet PCChessView *boardView;
    IBOutlet NSTextField *gameStatus;
    IBOutlet NSTextField *moveText;
	
	// commands
    IBOutlet id quitCommand;
	
	// model	
	pulchess::logic::Pulchess * gameFacade;
}
- (IBAction)acceptMove:(id)sender;
- (IBAction)newGame:(id)sender;
- (IBAction)quit:(id)sender;
- (void)gameStep:(id)sender;
- (void)updateChessBoard;
@end
