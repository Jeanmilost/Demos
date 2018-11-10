//
//  Maze_generatorAppDelegate.h
//  Maze generator
//
//  Created by Jean-Milost Reymond on 18.01.11.
//  Copyright 2011 WindSolutions. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Maze_generatorController.h"

@interface Maze_generatorAppDelegate : NSObject <NSApplicationDelegate>
{
    NSWindow*                 window;
    Maze_generatorController* m_pController;
}

@property (assign) IBOutlet NSWindow*                 window;
@property (assign) IBOutlet Maze_generatorController* m_pController;

@end
