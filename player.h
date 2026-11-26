/*------------------------------------------------------------
* File: player.h
*
* PURPOSE: Defines the player type that is used to spawn the player
           character (direction, location). Declares functions to 
           spawn and move the player
*
* Course:  COMP 2659
* Team Members: Matthew Kim, Ujjwal Bhandari
* Author: Matthew Kim
*------------------------------------------------------------*/


#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

/*player directions*/
#define NORTH 0
#define EAST  1
#define SOUTH 2
#define WEST 3

/* PLAYER
* purpose: the struct we will use for both the player and the 'ally' entity
*
* x: the x coordinate of the player
* y: the y coordinate of the player
* facing: the direction the player is facing (0-3, see above)
*/
typedef struct {
    UINT8 x; 
    UINT8 y;
    UINT8 facing;
} Player;

/* Declartions for .c*/

/*----- Function: player_set_position -----
* PURPOSE:  sets the player to a new position. I chose this instead of "moving" the player 1 or two tiles because it makes
*           more sense for slip tiles we will implament
*
* INPUT:    player - the player to be moved
*           x, y   - the coordinates of the new position
*
* OUTPUT:   None
*
* NOTE:     I chose this instead of "moving" the player 1 or two tiles because it makes
*           more sense for slip tiles we will implament
*/
void player_set_position(Player* player, UINT8 x, UINT8 y);

/*----- Function: player_forward_cell -----
* PURPOSE:   returns the cell infront of the player. 
*            This is important due to the inclusion of slip and wall tiles
*
* INPUT:    player - a Player object facing some tile
*           next_x - pointer to the x value of the tile in front of the player
*           next_y - pointer to the x value of the tile in front of the player
*
*OUTPUT:    None
*/
void player_forward_cell(const Player* player, INT16* next_x, INT16* next_y);

/*----- Function: rotate_right-----
* PURPOSE:   rotates the player right of current heading
*
* INPUT:     player - a Player object whose heading will be updated
*
* OUTPUT:    None
*/
void player_rotate_right(Player* player);

/*----- Function: rotate_left -----
* PURPOSE:   rotates the player left of current heading
*
* INPUT:     player - a Player object whose heading will be updated
*
* OUTPUT: None
*/
void player_rotate_left(Player* player);

/*----- Function: player_turn_around-----
* PURPOSE:   purpose rotates the player 180 degree
*
* INPUT:     player - a Player object whose heading will be updated
*
* OUTPUT:    None
*/
void player_turn_around(Player* player);

/*----- Function: player_init -----
* PURPOSE:   initializes a player struct with set position and facing values
*
* INPUT:     x, y - the x and y position of the player.
*            facing - the heading of the player (0-3 as outlined in player.h)
*
* OUTPUT:    None
*/
/* PLAYER_INIT
* purpose: initializes a player struct with set position and facing values
*/
void player_init(Player* player, UINT8 x, UINT8 y, UINT8 facing);

#endif