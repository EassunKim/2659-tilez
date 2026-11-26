#ifndef EFFECTS_H
#define EFFECTS_H
/*------------------------------------------------------------
* File: effects.h
*
* PURPOSE: This file is responsible for the sound effects in the game. play_move and play_fire.
*
* NOTES:   As of the submission of checkpoint 4, the two sound effects mentioned above were chosen as our
           two required sound effects. More can/will be added for the next submission following the exact same 
           structure as these two.
*
* Course:  COMP 2659
* Team Members: Matthew Kim, Ujjwal Bhandari
* Author: Matthew Kim
*------------------------------------------------------------*/

/*----- Function: play_move -----
* PURPOSE: Uses channel C to play the sound effect for movement
*
* INPUT: none
*
* OUTPUT: none
*/
void play_move(void);

/*----- Function: play_fire -----
* PURPOSE: Uses channel C to play the sound effect for colliding with a fire tile.
           Uses Noise instead of tone.
*
* INPUT: none
*
* OUTPUT: none
*/
void play_fire(void);

#endif