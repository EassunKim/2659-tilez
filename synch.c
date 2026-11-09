#include "synch.h"

void synch_tick(Model *model) {
    if (model->level_complete)
        return;

    model_tick(model);
}

void synch_req(Model *model) {
    if (model->input == REQ_FORWARD) {
        model_move_forward(model, &model->player_moved, &model->ally_moved);
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
