#include <stddef.h>
#include "model.h"

void model_reset_level(Model *model) {
    model->player = model->player_spawn;
    model->ally = model->ally_spawn;

}

void model_load_level(Model *model, UINT8 select) {
    const Level *lvl;

    /*load level*/
    lvl = level_get(select);
    level_build_grid(lvl, model->grid);

    /*set spawns from level layout*/
    model->player_spawn = lvl->player_spawn;
    model->ally_spawn = lvl->ally_spawn;

    /*set trackers*/
    model->move_count = 0;
    model->level_complete = FALSE;
    model->level_select = select;

    /* time (just to have a sync)*/
    model->tick_count = 0;
    model->seconds = 0;

    /* checkpoint 3 movements*/
    model->input = REQ_NONE;
    model->player_moved = FALSE;
    model->ally_moved = FALSE;
    
    model_reset_level(model);
    return;

}

UINT8 model_tile_at(const Model *model, INT16 x, INT16 y) {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
        return T_VOID;
    }

    return model->grid[y][x].type;
}

BOOL model_can_enter(const Model *model, const Player *character, const Player *other) {
    INT16 next_x;
    INT16 next_y;
    UINT8 opposite; /*the opposite direction of the way character is currently facing*/

    player_forward_cell(character, &next_x, &next_y);
    opposite = (character->facing +2) % 4;

    /*tile checks
    * don't allow movement if void or wall fall infront of a player entity.
    */
   /*check if void*/
    if (model_tile_at(model, next_x, next_y) == T_VOID)
        return FALSE;

    /*check wall*/
    if (tile_check_walls(&model->grid[character->y][character->x], character->facing))
        return FALSE;

    /* check opposite wall*/
    if (tile_check_walls(&model->grid[next_y][next_x], opposite))
        return FALSE;

    /*check if other character is blocking movement*/
    if (other->x == next_x && other->y == next_y)
        return FALSE;

    return TRUE;
}

/* ------------------------- movement ---------------------------- */

/*----- Function: try_step -----
 PURPOSE: Moves a character one cell forward if model_can_enter allows it.

 INPUT: model - the model the character is in
        mover - the character to move
        other - the other character

 OUTPUT: TRUE if the character moved, FALSE if it was blocked
*/
static BOOL try_step(Model *model, Player *mover, const Player *other) {
    INT16 next_x;
    INT16 next_y;

    if (!model_can_enter(model, mover, other)) {
        return FALSE;
    }
    player_forward_cell(mover, &next_x, &next_y);
    player_set_position(mover, next_x, next_y);
    return TRUE;
}

void model_move_forward(Model *model, BOOL *player_moved, BOOL *ally_moved) {
    *player_moved = try_step(model, &model->player, &model->ally);
    *ally_moved = try_step(model, &model->ally, &model->player);

    if (*player_moved || *ally_moved) {
        model->move_count++;
    }
}

void model_rotate_left(Model *model) {
    player_rotate_left(&model->player);
    player_rotate_left(&model->ally);
}

void model_rotate_right(Model *model) {
    player_rotate_right(&model->player);
    player_rotate_right(&model->ally);
}

void model_turn_around(Model *model) {
    player_turn_around(&model->player);
    player_turn_around(&model->ally);
}

BOOL model_slip(Model *model, Player *who, const Player *other) {
    return try_step(model, who, other);
}

void model_tick(Model *model) {
    model->tick_count++;

    if (model->tick_count >= TICKS_PER_SECOND) {
        model->tick_count = 0;
        model->seconds++;
    }
}


