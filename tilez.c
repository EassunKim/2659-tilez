/*------------------------------------------------------------
* File: tilez.c
*
* PURPOSE: The main game module of our game TileZ
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

/*Screen buffer size amd memory alignment*/
#define SCREEN_SIZE 32000
#define SCREEN_ALIGN 256

/*Allocated memory for back buffer*/
static UINT8 screen_memory[SCREEN_SIZE + SCREEN_ALIGN - 1];

/*Model snapshots for each screen buffer*/
static Model screen_models[2];

/*Screen buffer poiner and their model snapshots*/
typedef struct {
    UINT8 *front;
    UINT8 *back;
    UINT8 *original;
    Model *front_snapshot;
    Model *back_snapshot;
} ScreenBuffers;


/*----- Function: init_buffers-----
* PURPOSE: Initializes front and back screen buffers, and their snapshots
*          
*
* INPUT:   buffers  - Screen buffer structure being initalized
*          model    - Current game model
*
* OUTPUT:  Initializes both buffers and their snapshots
*
* ASSUMPTIONS: Screen memory large enough for alignment
*/
static void init_buffers(ScreenBuffers *buffers, const Model *model){
    UINT32 address;

    /*Save original screen buffer*/
    buffers->original = (UINT8 *)Physbase();

    /*Align back buffer to a 256 byte boundary*/
    address = (UINT32)screen_memory;
    address = (address + 255) & 0xFFFFFF00UL;

    buffers->front = buffers->original;
    buffers->back = (UINT8 *)address;

    
    /*Model snapshot for each buffer*/
    buffers->front_snapshot = &screen_models[0];
    buffers->back_snapshot = &screen_models[1];

    /*render initial game state to both buffers */
    render(model, buffers->front);
    render(model, buffers->back);

    /*Store initial model state for buffers*/
    *buffers->front_snapshot = *model;
    *buffers->back_snapshot = *model;
}


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


/*----- Function: flip_buffers-----
* PURPOSE: Switches front/back buffers and their model snapshots
*          
*
* INPUT:   buffers  - Screen buffer structure being updated
*
* OUTPUT:  Updated buffers and snapshot pointers
*
* ASSUMPTIONS: Both screens initialized
*/
static void flip_buffers(ScreenBuffers *buffers){
    UINT32 time_before;
    UINT8 *temp;
    Model *temp_snapshot;

    time_before = get_time();

    /*Schedule the screen flip*/
    Setscreen(-1L, (long)buffers->back, -1); 

    /* wait for vertical blank*/
    while (get_time() == time_before){
    }

    /*Swap the front/back buffer pointers*/
    temp = buffers->front;
    buffers->front = buffers->back;
    buffers->back = temp;

    /*Swap their model snapshots*/
    temp_snapshot = buffers->front_snapshot;
    buffers->front_snapshot = buffers->back_snapshot;
    buffers->back_snapshot = temp_snapshot;
}

/*----- Function: restore_screen -----
* PURPOSE: Restores the original frame buffer before exiting the game
*
* INPUT: buffers - Screen buffer structure containing the original screen address
*
* OUTPUT: Restores the original fram buffer
*/
static void restore_screen(ScreenBuffers *buffers){

    UINT32 time_before = get_time();

    /*Schedule the restoration of the original screen*/
    Setscreen(-1L, (long)buffers->original, -1); 

    while (get_time() == time_before){
    }

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
    static Model model;
    static ScreenBuffers buffers;
    BOOL quit = FALSE; /* will close the game */
    UINT32 time1, time2; /* The old/new time in the loop*/
    
    model_load_level(&model,0);
    init_buffers(&buffers, &model);
    
    time1 = get_time();
    while (quit == FALSE) {
        if (has_input()) 
            quit = use_input(&model, get_input());
        time2 = get_time();

        if (time1 != time2) {
            synch_req(&model);
            synch_tick(&model);
            cond_check(&model);
            
            render_changes(&model, buffers.back_snapshot, buffers.back); /*render changes to back buffer*/
            *buffers.back_snapshot = model; /*Save model states and flip buffers*/
            flip_buffers(&buffers);
            time1 = time2;
        }
    }
    restore_screen(&buffers); /*restore original screen before exit*/
    return 0;
}
