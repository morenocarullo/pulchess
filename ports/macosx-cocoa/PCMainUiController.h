/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo moreno.carullo [at] pulc [dot] it
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Main game UI controller interface
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

/* PCMainUiController */

#import <Cocoa/Cocoa.h>
#import "PCChessView.h"
#import "../../src/logic/pulchess.H"

@interface PCMainUiController : NSObject
{
	// view
    IBOutlet PCChessView *boardView;
    IBOutlet NSTextField *gameStatus;
    IBOutlet NSTextField *moveText;
    IBOutlet NSTextView  *gameHistoryView;
	
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
