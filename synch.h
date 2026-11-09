#ifndef SYNCH_H
#define SYNCH_H

#include "model.h"



/* Declarations */
/*----- Function: synch_tick -----
 PURPOSE: Handles one clock tick (70 per second). Advances the level timer.

 INPUT: model - the model to update

 OUTPUT: None

 NOTE: Does nothing once the level is complete, so the timer stops on a win.
*/
void synch_tick(Model *model);

/*----- Function: synch_req -----
 *
 * PURPOSE: will look at the qeueued action in the model and execute it on a clock tick
 *
 * INPUT: model - the model to update
 *
 * OUTPUT: None.
 */
void synch_req(Model *model);

#endif
