/* PCChessView */

#import <Cocoa/Cocoa.h>

@interface PCChessView : NSView
{
	//! Array dei pezzi sulla scacchiera
	NSImage * pezzi[64];
	
	//! Quant e' larga una casella?
	int dimPezzo;
}
- (void) setPiece:(char)pezzo col:(char)colore x:(int)cellX y:(int)cellY;
- (void) drawBoard;
@end
