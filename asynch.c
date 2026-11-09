#include "asynch.h"

/*----- Function: asynch_forward -----
 PURPOSE: Handles the forward key (W). Requests the player and ally to step forward.

 INPUT: model - the model to update

 OUTPUT: None
*/
void asynch_forward(Model *model) {
    if (model->level_complete)
        return;
    model->input = REQ_FORWARD;
}

/*----- Function: asynch_rotate_left -----
 PURPOSE: Handles the rotate-left key (A). Requests a rotation left

 INPUT: model - the model to update

 OUTPUT: None
*/
void asynch_rotate_left(Model *model) {
    if (model->level_complete)
        return;

    model->input = REQ_LEFT;
}

/*----- Function: asynch_rotate_right -----
 PURPOSE: Handles the rotate-right key (D). Requests a rotation right

 INPUT: model - the model to update

 OUTPUT: None

*/
void asynch_rotate_right(Model *model) {
    if (model->level_complete)
        return;

    model->input = REQ_RIGHT;
}

/*----- Function: asynch_turn_around -----
 PURPOSE: Handles the turn-around key (S). Requests a 180-degree turn for both characters.

 INPUT: model - the model to update

 OUTPUT: None
*/
void asynch_turn_around(Model *model) {
    if (model->level_complete)
        return;

    model->input = REQ_BACK;
}
