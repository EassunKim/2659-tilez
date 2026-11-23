#ifndef MODEL_H
#define MODEL_H


#include "types.h"
#include "player.h"
#include "tile.h"
#include "levels.h"

#define TICKS_PER_SECOND 70

/* Constants for input request values */
#define REQ_NONE 0
#define REQ_FORWARD 1
#define REQ_BACK 2
#define REQ_LEFT 3
#define REQ_RIGHT 4

/* The main game model. Holds and update info for the all entities on screen, input and levels*/
typedef struct {
    Tile grid[GRID_HEIGHT][GRID_WIDTH];
    Player player;
    Player ally;
    Player player_spawn;
    Player ally_spawn;
    UINT16 move_count;
    UINT8 tick_count;
    UINT16 seconds;
    BOOL level_complete;
    UINT8 level_select;
    /* checkpoint 3 movements for queing movements for asynch*/
    UINT8 input;
    BOOL player_moved;
    BOOL ally_moved;

} Model;


/*----- Function: model_reset_level -----
 PURPOSE: Puts the player and ally back on their spawns. Used after a fire tile.

 INPUT: model - the model to reset

 OUTPUT: None
*/
void model_reset_level(Model *model);

/*----- Function: model_load_level -----
 PURPOSE: Loads a level into the model: grid, spawns, counters, positions.

 INPUT: model - the model to load into
        select - which level to load

 OUTPUT: None
*/
void model_load_level(Model *model, UINT8 select);

/* ----------------- Checks ---------------------- */

/*----- Function: model_tile_at -----
 PURPOSE: Returns the type of the tile at a cell.

 INPUT: model - the model to read
        x, y  - the cell (may be off-grid)

 OUTPUT: the tile's type, or T_VOID if the cell is off the grid
*/
UINT8 model_tile_at(const Model *model, INT16 x, INT16 y);

/*----- Function: model_can_enter -----
 PURPOSE: Checks whether a character can step one cell in the direction it faces.

 INPUT: model - the model to read
        character - the character trying to move
        other - the other character (blocks the cell it stands on)

 OUTPUT: TRUE if the move is allowed, FALSE if blocked by:
         - void / off-grid target
         - a wall on the mover's tile, facing side
         - a wall on the target tile, opposite side
         - the other character standing on the target

 NOTE:  in this function, opposite is used to get the wall that would be facing the player.
        ex. player moving north has to worry about the north of their own tile and the south wall
        of the next tile.
*/
BOOL model_can_enter(const Model *model, const Player *character, const Player *other);

/* ------------------------- movement ------------------------- */

/*----- Function: model_move_forward -----
 PURPOSE: Handles a forward input: player steps, then ally steps.

 INPUT: model - the model to update

 OUTPUT: player_moved - set to TRUE if the player stepped, FALSE if blocked
         ally_moved   - set to TRUE if the ally stepped, FALSE if blocked

 ASSUMPTIONS: The player resolves first; the ally is checked against the
              player's NEW position. move_count goes up only if at least
              one character moved.
*/
void model_move_forward(Model *model, BOOL *player_moved, BOOL *ally_moved);

/*----- Function: model_rotate_left -----
 PURPOSE: Rotates both characters left. Does not count as a move.

 INPUT: model - the model to update

 OUTPUT: None
*/
void model_rotate_left(Model *model);

/*----- Function: model_rotate_right -----
 PURPOSE: Rotates both characters right. Does not count as a move.

 INPUT: model - the model to update

 OUTPUT: None
*/
void model_rotate_right(Model *model);

/*----- Function: model_turn_around -----
* PURPOSE: Turns both characters 180 degrees. Does not count as a move.
*
* INPUT: model - the model to update
*
* OUTPUT: None
*/
void model_turn_around(Model *model);

/*----- Function: model_slip -----
 PURPOSE: Slides a character one extra cell after landing on a slip tile.
          Called from cond.c.

 INPUT: model - the model the character is in
        who   - the character on the slip tile
        other - the other character

 OUTPUT: TRUE if the character slid, FALSE if blocked (it stays on the slip tile)
*/
BOOL model_slip(Model *model, Player *who, const Player *other);

/*----- Function: model_tick -----
 PURPOSE: Advances the level timer by one clock tick. Every TICKS_PER_SECOND
          ticks, adds one second.

 INPUT: model - the model to update

 OUTPUT: None

 ASSUMPTIONS: seconds wraps to 0 after about 18 hours on one level.
*/
void model_tick(Model *model);


#endif