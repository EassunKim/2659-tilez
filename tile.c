/*------------------------------------------------------------
* File: tile.c
*
* PURPOSE: Implements the functions outlines in tile.h
*
* Course:  COMP 2659
* Team Members: Matthew Kim, Ujjwal Bhandari
* Author: Matthew Kim
*------------------------------------------------------------*/


#include "tile.h"


void tile_init(Tile* tile, UINT8 type, UINT8 walls) {
    tile->type = type;
    tile->walls = walls;
}

BOOL tile_check_walls(const Tile* tile, UINT8 side) {
    return (tile->walls & (1 << side)) != 0;
}

void tile_add_wall(Tile* tile, UINT8 side) {
    tile->walls |= (1 << side);
}


