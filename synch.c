#include "synch.h"

void synch_tick(Model *model) {
    if (model->ally_blocked != 0) {
        model->ally_blocked -= 1;
    }
    if (model->player_blocked != 0) {
        model->player_blocked -= 1;
    }

    if (model->level_complete)
        return;

    model_tick(model);
}

void synch_req(Model *model) {
    if (model->input == REQ_FORWARD) {
        model_move_forward(model, &model->player_moved, &model->ally_moved);
        if (model->player_moved == FALSE) {
            model->player_blocked = BLOCKED_TICKS;
        }
        if (model->ally_moved == FALSE) {
            model->ally_blocked = BLOCKED_TICKS;
        }
    }
    if (model->input == REQ_LEFT) {
        model_rotate_left(model);

    }
    if (model->input == REQ_RIGHT) {
        model_rotate_right(model);
    }
    if (model->input == REQ_BACK) {
        model_turn_around(model);
    }
    if (model->input == REQ_NONE) {
        return;
    }
    model->input = REQ_NONE;
    return;

}
