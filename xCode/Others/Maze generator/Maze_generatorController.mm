//
//  Maze_generatorController.m
//  Maze generator
//
//  Created by Jean-Milost Reymond on 18.01.11.
//  Copyright 2011 WindSolutions. All rights reserved.
//

#import "Maze_generatorController.h"
#include "E_Maze.h"

@implementation Maze_generatorController

- (void)Generate :(unsigned)width :(unsigned)height
{
    NSFont* pFont = [NSFont fontWithName:@"Courier New" size:12];

    if (!pFont)
        throw "Prepare fonts - cannot create big font object";

    [m_Text setFont:pFont];

    E_Maze maze(20, 20);

    NSString* pStr = [[NSString alloc]init];

    for (unsigned y = 0; y < maze.GetHeight(); ++y)
    {
        for (unsigned x = 0; x < maze.GetWidth(); ++x)
            switch (maze.Get(x, y).m_Type)
            {
                case E_Maze::ICell::IE_Empty:
                    pStr = [pStr stringByAppendingString:@"."];
                    break;

                case E_Maze::ICell::IE_Wall:
                    pStr = [pStr stringByAppendingString:@"X"];//@"∑"];
                    break;

                case E_Maze::ICell::IE_Door:
                    pStr = [pStr stringByAppendingString:@"."];
                    break;

                case E_Maze::ICell::IE_Start:
                    pStr = [pStr stringByAppendingString:@"S"];
                    break;

                case E_Maze::ICell::IE_End:
                    pStr = [pStr stringByAppendingString:@"E"];
                    break;

                case E_Maze::ICell::IE_Key:
                    pStr = [pStr stringByAppendingString:@"K"];
                    break;

                default:
                    throw L"Cell type does not exist";
            }

        pStr = [pStr stringByAppendingString:@"\n"];
    }

    [m_Text setStringValue:pStr];
}

@end
