/*
 * PROJECT: PulCHESS, a Computer Chess program
 * AUTHOR:  Moreno Carullo moreno.carullo [at] pulc [dot] it
 * LICENSE: GPL, see license.txt in project root
 * FILE:    Game thread utility
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
#import "PCGameThread.h"
#import "PCMainUiController.h"


@implementation PCGameThread
+ (void)DoThinking:(id)anObject {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	[anObject performSelector:@selector(gameStep:) withObject:anObject];
	//NSLog(@"Ok dal thread");
	
	[pool release];
}
@end
