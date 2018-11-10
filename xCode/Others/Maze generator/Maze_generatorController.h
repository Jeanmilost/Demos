//
//  Maze_generatorController.h
//  Maze generator
//
//  Created by Jean-Milost Reymond on 18.01.11.
//  Copyright 2011 WindSolutions. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface Maze_generatorController : NSObject
{
    IBOutlet id m_Text;
}

- (void)Generate :(unsigned)width :(unsigned)height;

@end
