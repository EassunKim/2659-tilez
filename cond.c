#include "cond.h"

/*----- Function: apply_slip -----
 PURPOSE: Slides a character one extra cell if it stepped onto a slip tile.

 INPUT: model - the model the character is in
        who   - the character to check
        other - the other character (can block the slide)
        moved - TRUE if 'who' stepped this turn

 OUTPUT: None

*/
static void apply_slip(Model *model, Player *who, const Player *other, BOOL moved) {
    if (moved && model_tile_at(model, who->x, who->y) == T_SLIP) {
        model_slip(model, who,other);
    }
}

/*----- Function: check_fire -----
 PURPOSE: If either character is on a fire tile, resets both to their spawns.

 INPUT: model - the model to check

 OUTPUT: TRUE if a reset happened, FALSE otherwise
*/
static BOOL check_fire(Model *model) {
    if (model_tile_at(model, model->player.x, model->player.y) == T_FIRE ||
        model_tile_at(model, model->ally.x, model->ally.y) == T_FIRE) {
        model_reset_level(model);
        return TRUE;
    }
    return FALSE;
}

/*----- Function: check_goal -----
 PURPOSE: Marks the level complete if the ally is on the goal tile.

 INPUT: model - the model to check

 OUTPUT: None
*/
static void check_goal(Model *model) {
    if (model_tile_at(model, model->ally.x, model->ally.y) == T_GOAL)
        model->level_complete = TRUE;
}

void cond_after_move(Model *model, BOOL player_moved, BOOL ally_moved) {
    apply_slip(model, &model->player, &model->ally, player_moved);
    apply_slip(model, &model->ally, &model->player, ally_moved);

    if (check_fire(model))
        return;

    check_goal(model);
}

void cond_check(Model *model) {
    cond_after_move(model, model->player_moved, model->ally_moved);
    model->player_moved = FALSE;
    model->ally_moved = FALSE;
}
