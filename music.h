/*------------------------------------------------------------
* File: music.h
*
* PURPOSE: Plays a looping song, (VERY simple since we're in comp sci) on Channel A of the PSG
*
*
* Course:  COMP 2659
* Team Members: Matthew Kim, Ujjwal Bhandari
* Author: Matthew Kim
*------------------------------------------------------------*/

#ifndef MUSIC_H
#define MUSIC_H

#include "types.h"

/*----- Function: start_music-----
* PURPOSE: starts the song from the beginning. Takes channel A and sets the appropriate PSG settings.
*
* INPUT: NONE
*
* OUTPUT: NONE
*/
void start_music(void);

/*----- Function: update_music -----
* PURPOSE: moves through the song by switching notes and looping back to the start
*
* INPUT: ticks - The number of ticks since the last update call (used for note duration)
*
* OUTPUT: none
*
* ASSUMPTIONS: This function does not select a channel, or start the track, this is all done in the setup
*       during the start_music call.
*/
void update_music(UINT32 ticks);

#endif