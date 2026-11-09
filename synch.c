#include "synch.h"

/*----- Function: synch_tick -----
 PURPOSE: Handles one clock tick (70 per second). Advances the level timer.

 INPUT: model - the model to update

 OUTPUT: None

 NOTE: Does nothing once the level is complete, so the timer stops on a win.
*/
void synch_tick(Model *model) {
    if (model->level_complete)
        return;

    model_tick(model);
}

void synch_req(Model *model) {
    if (model->input == REQ_FORWARD) {
        model_move_forward(model, &model->player_moved, &model->ally_moved);
        model->input = REQ_NONE;
        return;
    }
    if (model->input == REQ_LEFT) {
        model_rotate_left(model, &model->player_moved, &model->ally_moved);
        model->input = REQ_NONE;
        return;
    }
    if (model->input == REQ_RIGHT) {
        model_rotate_right(model, &model->player_moved, &model->ally_moved);
        model->input = REQ_NONE;
        return;
    }
    if (model->input == REQ_BACK) {
        model_turn_around(model, &model->player_moved, &model->ally_moved);
        model->input = REQ_NONE;
        return;
    }
    if (model->input == REQ_NONE) {
        return;
    }

}
