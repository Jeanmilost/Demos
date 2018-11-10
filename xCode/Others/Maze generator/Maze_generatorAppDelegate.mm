//
//  Maze_generatorAppDelegate.m
//  Maze generator
//
//  Created by Jean-Milost Reymond on 18.01.11.
//  Copyright 2011 WindSolutions. All rights reserved.
//

#import "Maze_generatorAppDelegate.h"

@implementation Maze_generatorAppDelegate

@synthesize window;
@synthesize m_pController;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    if (m_pController)
        [m_pController Generate :20 :20];
}

@end
