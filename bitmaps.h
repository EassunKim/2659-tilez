#ifndef BITMAPS_H
#define BITMAPS_H

#include "types.h"

#define TILE_HEIGHT  32
#define CHAR_HEIGHT  16
#define WALL_ATSU    4

#define WALL_LENGTH  36 /* extending the walls to 36 for a consistent look when walls are rendered*/

extern const UINT32 tile_bitmaps[5][TILE_HEIGHT];   /* the index will match the tile type in tiles.h; 0 for void etc.  */
extern const UINT16 player_bitmaps[4][CHAR_HEIGHT]; /* index matches directions in tiles.h  */
extern const UINT16 ally_bitmaps[4][CHAR_HEIGHT];   /* same as above, but sprites for the ally */

extern const UINT32 wall_horizontal[WALL_ATSU];     /*extra layers to the horizontal walls*/
extern const UINT32 wall_vertical[WALL_LENGTH];   /* 36 rows of 0xF0000000UL to thicken the horizontal walls */
#endif
