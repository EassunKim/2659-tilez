/*------------------------------------------------------------
* File: tilez.c
*
* PURPOSE: The main game module of for our game TileZ
*
* NOTES:  This is for the checkpoint 3 submission. It will contain the main game loop and 
*          double buffering.
*
* Course:  COMP 2659
* Team Members: Matthew Kim, Ujjwal Bhandari
* Author: Matthew, Ujjwal
*------------------------------------------------------------*/

#include <osbind.h> 
#include "input.h"
#include "model.h"
#include "renderer.h"
#include "events.h"


/*----- Function: get_time -----
* PURPOSE: Retrieves the current time from the system timer
*
* INPUT: None
*
* OUTPUT: The current time value
*/
UINT32 get_time(void) {
    long* timer_add = (long *)0x462;
    long old_sp = Super(0);
    long time_val = *(long *)timer_add;

    Super(old_sp);
    return time_val;
}

/*----- Function: use_input -----
* PURPOSE: take an input during the main game loop and applies it to the 
*
* INPUT: model - the model to update
         input - a character input from the user to apply
*
* OUTPUT: BOOL - TRUE if the input was applied, FALSE if it was not a valid input. 
*/
static BOOL use_input(Model *model, char input) {
    switch (input) {
        case 'w': case 'W': 
        asynch_forward(model);
        break;
        case 'a': case 'A': 
        asynch_rotate_left(model);
        break;
        case 'd': case 'D': 
        asynch_rotate_right(model);
        break;
        case 's': case 'S': 
        asynch_turn_around(model);
        break;
        case 'q': case 'Q': 
        return TRUE; /*used as a "quit" to break the loop*/
    }
    return FALSE;
}


/*----- Function: main -----
* PURPOSE: the main game loop for tilez
*
* INPUT: None
*
* OUTPUT: returns 0 after the loop
*/
int main(void) {
    Model model;
    UINT8 *base = (UINT8 *)Physbase();
    BOOL quit = FALSE; /* will close the game */
    UINT32 time1; /* The old time in the loop*/
    UINT32 time2; /* The new time in the loop*/
    model_load_level(&model,0);
    render(&model, base);
    
    time1 = get_time();
    while (quit == FALSE) {
        if (has_input()) {
            quit = use_input(&model, get_input());
        }
        time2 = get_time();
        if (time1 != time2) {
            synch_req(&model);
            synch_tick(&model);
            cond_check(&model);
            render(&model, base);
            time1 = time2;
        }
    }
    
    return 0;
}
