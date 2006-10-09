#import "PCChessView.h"
#import <string.h>

@implementation PCChessView

- (void)awakeFromNib
{
	int i=0;
	for(;i<64;i++) {
		pezzi[i] = nil;
		//pezzi[i] = [NSImage imageNamed:@"black-pawn"];
	}
}

- (id)initWithFrame:(NSRect)frameRect
{
	if ((self = [super initWithFrame:frameRect]) != nil) {
		dimPezzo = 50;
	}
	return self;
}

- (void)drawRect:(NSRect)rect
{		
	[self drawBoard];
}

// imposta il pezzo alla posizione specificata
- (void) setPiece:(char)pezzo col:(char)colore x:(int)cellX y:(int)cellY
{
	NSImage  *img = nil;
	NSString *imgPath;
	char buff[10], dash = '-', *piece="piece";
	strcpy(buff, piece);
	strncpy(buff+5, &dash,	1);
	strncpy(buff+6, &colore,1);
	strncpy(buff+7, &dash,	1);
	strncpy(buff+8, &pezzo,	1);
	buff[9] = '\0';
	imgPath = [NSString stringWithCString:buff];

	if( pezzo != '-' ) {
		img = [NSImage imageNamed:imgPath];
		pezzi[cellX*8 + cellY] = img;
	}
	else {
		pezzi[cellX*8 + cellY] = nil;
	}
}

//! Disegna la scacchiera vuota, senza alcun pezzo.
- (void)drawBoard
{
	int i=0, j=0;
	int idx=0;
	
	NSRect imgRect = NSMakeRect(0,0,dimPezzo,dimPezzo);
	for(i=0; i<8; i++)
	{
		for(j=0; j<8; j++) {
			// disegno della parte statica della board
			NSRect  r;
			NSPoint p;
			r = NSMakeRect(i*dimPezzo, j*dimPezzo, (i*dimPezzo)+dimPezzo,(j*dimPezzo)+dimPezzo);
			p = NSMakePoint(i*dimPezzo, j*dimPezzo);
			
			if( (i+j)%2 == 0 ) {
				[[NSColor grayColor] set];
			}
			else {
				[[NSColor lightGrayColor] set];
			}
						
			[NSBezierPath fillRect:r];
			
			idx = i*8 + j;
			if( pezzi[idx] != nil ) {	
				[pezzi[idx] drawAtPoint:p fromRect:imgRect operation:NSCompositeSourceOver fraction:1.0];
			}
		}
	}
}


@end
