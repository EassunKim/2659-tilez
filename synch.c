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

