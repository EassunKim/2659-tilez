#ifndef ASYNCH_H
#define ASYNCH_H

#include "model.h"

/*----- Function: asynch_forward -----
* PURPOSE: Handles the forward key (W). Requests the player and ally to step forward.
           As of Checkpoint 4, plays a sound on move.
*
* INPUT: model - the model to update
* 
* OUTPUT: None
*/
void asynch_forward(Model *model);

/*----- Function: asynch_rotate_left -----
* PURPOSE: Handles the rotate-left key (A). Requests a rotation left
*
* INPUT: model - the model to update
*
* OUTPUT: None
*/
void asynch_rotate_left(Model *model);

/*----- Function: asynch_rotate_right -----
* PURPOSE: Handles the rotate-right key (D). Requests a rotation right
*
* INPUT: model - the model to update
*
* OUTPUT: None
*/
void asynch_rotate_right(Model *model);

/*----- Function: asynch_turn_around -----
* PURPOSE: Handles the turn-around key (S). Requests a 180-degree turn for both characters.
*
* INPUT: model - the model to update
*
* OUTPUT: None
*/
void asynch_turn_around(Model *model);

#endif