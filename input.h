/*------------------------------------------------------------
* File: input.h
*
* PURPOSE: input.h/c hold the functions that will check for an input
*
*
* Course:  COMP 2659
* Team Members: Matthew Kim, Ujjwal Bhandari
* Author: Matthew Kim
*------------------------------------------------------------*/

#ifndef INPUT_H
#define INPUT_H

#include "types.h"

/*----- Function: has_input -----
* PURPOSE: checks if there is an input from the user and returns true if a key is waiting to be read
*
* INPUT: None
*
* OUTPUT: BOOL - TRUE if a key is waiting to be read, FALSE otherwise
*/
BOOL has_input(void);

/*----- Function: get_input -----
* PURPOSE: returns the next input from the user
*
* INPUT: None
*
* OUTPUT: char - The next input from the user
*/
char get_input(void);
#endif