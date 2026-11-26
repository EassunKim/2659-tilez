/*------------------------------------------------------------
* File: player.c
*
* PURPOSE: implements the functions declared in player.h 
*
* Course:  COMP 2659
* Team Members: Matthew Kim, Ujjwal Bhandari
* Author: Matthew Kim
*------------------------------------------------------------*/


#include "player.h"

void player_init(Player* player, UINT8 x, UINT8 y, UINT8 facing) {
    player->x = x;
    player->y = y;
    player->facing = facing;
}

/*---------------DIRECTION CHANGE FUNCTIONS --------------------*/

void player_rotate_left(Player* player) {
    player->facing = (player->facing + 3) % 4; /*using mod to 'wrap' the value back around to 0-3 if exceeded*/
}

void player_rotate_right(Player* player) {
    player->facing = (player->facing + 1) % 4;
}

void player_turn_around(Player* player) {
    player->facing = (player->facing + 2) % 4;
}

/*-----------PLAYER MOVEMENT FUNCTIONS-----------*/

void player_forward_cell(const Player* player, INT16* next_x, INT16* next_y) {
    int x_directs[4] = {0,1,0,-1}; /* the value that next_x should be incremented by based on facing value of player*/
    int y_directs[4] = {-1,0,1,0}; /* same as above but for next_y*/

    *next_x = player->x + x_directs[player->facing];
    *next_y = player->y + y_directs[player->facing];
}

void player_set_position(Player* player, UINT8 x, UINT8 y) {
    player->x = x;
    player->y = y;
}