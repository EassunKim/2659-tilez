/*------------------------------------------------------------
* File: splash.h
*
* PURPOSE: Declares the headers that will be used to print our splash 
*
* Course:  COMP 2659
* Team Members: Matthew Kim, Ujjwal Bhandari
* Author: Matthew Kim
*------------------------------------------------------------*/
#ifndef SPLASH_H
#define SPLASH_H

#include "types.h"

/* trackers for the status of the splash screen */
#define SPLASH_PLAY 0
#define SPLASH_QUIT 1


/*----- Function: splash_screen -----
* PURPOSE: draws the main menu of the game and waits for user input
*
* INPUT: base - the screen buffer in which to draw the splash
*
* OUTPUT: UINT8 - one of the above states that is used to draw a square around the options
*/

UINT8 splash_screen(UINT8 *base);



#endif 