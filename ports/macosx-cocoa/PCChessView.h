/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo moreno.carullo [at] pulc [dot] it
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Chessboard NSView
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
 * Created on ?
 * $Id$
 */

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
