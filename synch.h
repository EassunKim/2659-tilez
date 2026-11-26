/*------------------------------------------------------------
* File: synch.h
*
* PURPOSE: Declares the synchronous event handlers, run once per clock
*          tick: carrying out the pending move request and advancing timers.
*
* Course:  COMP 2659
* Team Members: Matthew Kim, Ujjwal Bhandari
* Author: Matthew Kim
*------------------------------------------------------------*/


#ifndef SYNCH_H
#define SYNCH_H

#include "model.h"



/* Declarations */
/*----- Function: synch_tick -----
 PURPOSE: Handles one clock tick (70 per second). Advances the level timer and counts down desynced movement

 INPUT: model - the model to update

 OUTPUT: None
*/
void synch_tick(Model *model);

/*----- Function: synch_req -----
 *
 * PURPOSE: will look at the qeueued action in the model and execute it on a clock tick. Sets ally_blocked and
 *          player _blocked to solve the problem described in model.h
 *
 * INPUT: model - the model to update
 *
 * OUTPUT: None.
 */
void synch_req(Model *model);

#endif
