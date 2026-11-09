#ifndef COND_H
#define COND_H

#include "model.h"

/* Declarations */
/*----- Function: cond_after_move -----
 PURPOSE: Applies all tile effects after a forward move, in order:
          slips, then fire, then goal.

 INPUT: model - the model to update
        player_moved - TRUE if the player stepped this turn
        ally_moved   - TRUE if the ally stepped this turn

 OUTPUT: None

 NOTE:   Slips are resolved first so a slide onto fire is caught.
*/
void cond_after_move(Model *model, BOOL player_moved, BOOL ally_moved);


/* Declarations */
/*----- Function: cond_after_move -----
 PURPOSE: call to cond_after_move for the inout requests. Will do the condition check that was tested in 
          checkpoint 2 and will reset the player_moved and ally_moved flags to false after the check.

 INPUT: model - the model to update

 OUTPUT: None
*/
void cond_check(Model *model)

#endif
