/*------------------------------------------------------------
* File: asynch.c
*
* PURPOSE: Implements keypress handlers
*
* NOTES: Added the move sound effect as of checkpoint 4
*
* Course:  COMP 2659
* Team Members: Matthew Kim, Ujjwal Bhandari
* Author: Matthew Kim
*------------------------------------------------------------*/


#include "asynch.h"
#include "effects.h"

void asynch_forward(Model *model) {
    if (model->level_complete)
        return;
    play_move();
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
