#ifndef SYNCH_H
#define SYNCH_H

#include "model.h"



/* Declarations */
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
