/*------------------------------------------------------------
* File: tst_music.c
*
* PURPOSE: a light weight tester made to simply play our main theme to check note sound and spacing. 
           Also serves as a test for start_music and update_music in music.c
*
* Course:  COMP 2659
* Team Members: Matthew Kim, Ujjwal Bhandari
* Author: Matthew Kim
*------------------------------------------------------------*/

#include <osbind.h>
#include "music.h"
#include "psg.h"

/*----- Function: get_time -----
* PURPOSE: retrieves the current time from the system clock. Taken from our main game Tilez.c written by Ujjwal
*
* INPUT: NONE
*
* OUTPUT: A UINT32 of the current time value.
*/
static UINT32 get_time(void) {
    long *timer_add = (long *)0x462;
    long old_sp = Super(0);
    long time_val = *timer_add;
    Super(old_sp);
    return time_val;
}

/*----- Function: main -----
* PURPOSE: plays our main theme and stops on key press
*
* INPUT: NONE
*
* OUTPUT: 0
*/

int main(void) {
    UINT32 time1;
    UINT32 time2;

    Cconws("Playing music. Press any key to stop.\r\n");

    start_music();
    time1 = get_time();

    while (!Cconis()) {
        time2 = get_time();
        if (time2 != time1) {
            update_music(time2 - time1);
            time1 = time2;
        }
    }

    Cnecin();
    stop_sound();
    return 0;
}