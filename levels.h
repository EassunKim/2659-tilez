/*------------------------------------------------------------
* File: levels.h
*
* PURPOSE: Defines the level input that we will use to layout all game levels. 
           Declares the functions to build a level and build its layout
*
* NOTES:   
*
* Course:  COMP 2659
* Team Members: Matthew Kim, Ujjwal Bhandari
* Author: 
*------------------------------------------------------------*/


#ifndef LEVELS_H
#define LEVELS_H

#include "types.h"
#include "tile.h"
#include "player.h"

#define LEVELS 1
#define GRID_WIDTH  10
#define GRID_HEIGHT 10


/*wall
Defines the tile a wall is on and the side of that wall it is facing/blocking
*/
typedef struct {
    UINT8 x;
    UINT8 y;
    UINT8 side;
} wallInfo;

/*Level
Defines a full level layout including the spawns, tile layout and walls
*/
typedef struct {
    const char *rows[GRID_HEIGHT];
    const wallInfo *walls;
    const UINT8 wall_count;
    Player player_spawn;
    Player ally_spawn;
} Level;

/*----- Function: level_get -----
* PURPOSE: retrieves a level from the list at a given index
*
* INPUT: index - the level to retrieve
*
* OUTPUT: A level pointer, as specified above
*/
const Level *level_get(UINT8 index);

/*LEVEL BUILDING*/

/*----- Function: level_build_grid-----
 PURPOSE: translates our grid into the tile types for level making

 INPUT: lvl - a level array to build
        grid - a 2d array of Tile objects (see tile.h) to initialize

 OUTPUT: None
*/
void level_build_grid(const Level *lvl, Tile grid[GRID_HEIGHT][GRID_WIDTH]);




#endif