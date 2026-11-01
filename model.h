#ifndef MODEL_H
#define MODEL_H


#include "types.h"
#include "player.h"
#include "tile.h"
#include "levels.h"

#define TICKS_PER_SECOND 70

typedef struct {
    Tile grid[GRID_HEIGHT][GRID_WIDTH];
    Player player;
    Player ally;
    Player player_spawn;
    Player ally_spawn;
    UINT16 move_count;
    UINT8 tick_count;
    UINT16 seconds;
    BOOL level_complete;
    UINT8 level_select;
} Model;


/*declarations, function specifications are above each function in the .c*/
BOOL model_slip(Model *model, Player *who, const Player *other);
void model_turn_around(Model *model);
void model_rotate_right(Model *model);
void model_rotate_left(Model *model);
void model_move_forward(Model *model, BOOL *player_moved, BOOL *ally_moved);
BOOL model_can_enter(const Model *model, const Player *character, const Player *other);
UINT8 model_tile_at(const Model *model, INT16 x, INT16 y) ;
void model_load_level(Model *model, UINT8 select);
void model_reset_level(Model *model);
void model_tick(Model *model);

#endif