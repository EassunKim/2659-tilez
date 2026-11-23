#include "asynch.h"

void asynch_forward(Model *model) {
    if (model->level_complete)
        return;
    model->input = REQ_FORWARD;
}

void asynch_rotate_left(Model *model) {
    if (model->level_complete)
        return;

    model->input = REQ_LEFT;
}

void asynch_rotate_right(Model *model) {
    if (model->level_complete)
        return;

    model->input = REQ_RIGHT;
}

void asynch_turn_around(Model *model) {
    if (model->level_complete)
        return;

    model->input = REQ_BACK;
}
