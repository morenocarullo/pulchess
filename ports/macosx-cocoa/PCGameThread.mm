//
//  PCGameThread.m
//  PulCHESSX
//
//  Created by Moreno Carullo on 07/02/06.
//  Copyright 2006 __MyCompanyName__. All rights reserved.
//

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
