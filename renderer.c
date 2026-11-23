
#include "renderer.h"
#include "raster.h"
#include "types.h"

#include "bitmaps.h"

/* Title printing */
#define SCREEN_WIDTH 640
#define FONT_WIDTH 8
#define TEXT_COL 300 /* centered */
#define LEVEL_ROW 376
#define TITLE_ROW 16

/*----- Function: animate -----
* PURPOSE: based on the clock ticks (0 and 1/2 through the cycle), checks the animation state
*          (declared as anim_state in each function it is used) of the model.
*
* INPUT:   model - the game state to draw
*
* OUTPUT:  a UINT8 that is 0 when model ticks is below 1/2 and 1 otherwise
*/
static UINT8 animate(const Model *model) {
    UINT8 ticks = model->tick_count;
    if (ticks < (TICKS_PER_SECOND/2)) {
        return 0;
    } else {
        return 1;
    }
}

void render_tile(const Tile *tile, UINT8 *base, int row, int col, UINT8 anim_state)
{
    const UINT32 *tile_map = tile_bitmaps[tile->type];

    if (anim_state) {
        tile_map = tile_alt_bitmaps[tile->type];
    }
    plot_32bit_bitmap((UINT32 *)base, row, col, tile_map, TILE_HEIGHT);
    render_walls(tile, base, row, col);
}


void render_walls(const Tile *tile, UINT8 *base, int row, int col)
{
    if (tile->walls & WALL_NORTH)
        plot_32bit_bitmap((UINT32 *)base,
                          row, col,
                          wall_horizontal, WALL_ATSU);

    if (tile->walls & WALL_SOUTH)
        plot_32bit_bitmap((UINT32 *)base,
                          row + TILE_HEIGHT - WALL_ATSU, col,
                          wall_horizontal, WALL_ATSU);

    if (tile->walls & WALL_EAST)
        plot_32bit_bitmap((UINT32 *)base,
                          row + WALL_OFFSET, col + TILE_WIDTH - 2,
                          wall_vertical, WALL_LENGTH);

    if (tile->walls & WALL_WEST)
        plot_32bit_bitmap((UINT32 *)base,
                          row + WALL_OFFSET, col,
                          wall_vertical, WALL_LENGTH);
}

void render_board(const Tile board[GRID_HEIGHT][GRID_WIDTH], UINT8 *base, UINT8 anim_state)
{
    int row, col;

    for (row = 0; row < GRID_HEIGHT; row++)
    {
        for (col = 0; col < GRID_WIDTH; col++)
        {
            render_tile(&board[row][col], base,
                        40 + row * TILE_HEIGHT,
                        160 + col * TILE_WIDTH,
                        anim_state);
        }
    }
}


void render_player(const Player *player, UINT8 *base, BOOL blocked, UINT8 anim_state)
{
    int row, col;
    const UINT16 *player_maps = player_bitmaps[player->facing];

    row = 40 + player->y * TILE_HEIGHT;
    col = 160 + player->x * TILE_WIDTH;

    if (blocked) {
        player_maps = player_blocked_bitmaps[player->facing];
    } else if (anim_state)
    {
         player_maps = player_alt_bitmaps[player->facing];
    }

    plot_16bit_bitmap((UINT16 *)base, row+ CHAR_OFFSET, col+ CHAR_OFFSET, player_maps, CHAR_HEIGHT);
}

void render_ally(const Player *player, UINT8 *base, BOOL blocked, UINT8 anim_state)
{
    int row, col;
    const UINT16 *ally_maps = ally_bitmaps[player->facing];

    if (blocked) {
        ally_maps = ally_blocked_bitmaps[player->facing];
    } else if (anim_state)
    {
        ally_maps = ally_alt_bitmaps[player->facing];
    }

    row = 40 + player->y * TILE_HEIGHT;
    col = 160 + player->x * TILE_WIDTH;

    plot_16bit_bitmap((UINT16 *)base, row + CHAR_OFFSET, col+ CHAR_OFFSET, ally_maps, CHAR_HEIGHT);
}

/*----- Function: redraw_tile -----
* PURPOSE: Redraws a single tile, including the type, walls, 
*          and any characters on it
* INPUT:   model    - Current game model
*          base     - Pointer to the screen buffer
*          row      - Tile row in the board
*          col      - Tile column
*
*
* OUTPUT:  Updates the screen buffer with a changed tile
*
*/
static void redraw_tile(const Model *model, UINT8 *base, int row, int col){
    int screen_row = 40 + row * TILE_HEIGHT;
    int screen_col = 160 + col * TILE_WIDTH;
    UINT8 anim_state = animate(model);

    /*Clear the previous tile*/
    clear_region((UINT32 *)base, screen_row, 
                screen_col, TILE_HEIGHT, TILE_WIDTH);

    /*Restore a tile*/
    render_tile(&model->grid[row][col], base,
                screen_row, screen_col, anim_state);

    /*Redraw player/ally if they are present*/
    if (model->player.x == col && model->player.y == row)
    {
        render_player(&model->player, base, model->player_blocked, anim_state);
    }

    if (model->ally.x == col && model->ally.y == row)
    {
        render_ally(&model->ally, base, model->ally_blocked, anim_state);
    }
}

/*----- Function: redraw_alts -----
* PURPOSE: Redraws tiles and players (every animated element)
*
* INPUT:   model    - model to update
*          base     - Start of the game buffer to draw into
*
* OUTPUT:  Updates the screen buffer with alt sprites
*/
static void redraw_alts(const Model *model, UINT8 *base){
    int row, col;

    /* checks if we are looking at an animated tile and redraws them depending*/
    for (row = 0; row < GRID_HEIGHT; row++)
    {
        for (col = 0; col < GRID_WIDTH; col++)
        {
            if (model->grid[row][col].type == T_FIRE ||
                model->grid[row][col].type == T_GOAL ||
                model->grid[row][col].type == T_SLIP )
            {
                redraw_tile(model, base, row, col);
            }
        }
    }
    redraw_tile(model, base, model->player.y, model->player.x);
    redraw_tile(model, base, model->ally.y, model->ally.x);
}

/*----- Function: redraw_player_changes -----
* PURPOSE: Redraws tiles when a player/ally moves or changes direction, or its blocked state changes
*          
*
* INPUT:   model        - Current game model
*          current      - Current player/ally state
*          previous     - Previous player/ally state
*          base         - Start of the game buffer to draw into
*          cur_blocked  - timer for how long the character is currently blocked for (20ticks -> 0)
*          prev_blocked - the blocked counter when this buffer was last drawn. If it differs from cur_blocked 
*                         (0 or !0), the sprite is redrawn.
*
*
* OUTPUT:  Updates the screen buffer with changed tiles
*
*/
static void redraw_player_changes(const Model *model, const Player *current, 
const Player *previous, UINT8 *base, UINT8 cur_blocked, UINT8 prev_blocked) {
    /*Player movement/direction change check*/
    if (current->x != previous->x || current->y != previous->y ||
            current->facing != previous->facing || (cur_blocked != 0) != (prev_blocked != 0))
    {
        /*restore previous tile, remove sprite*/
        redraw_tile(model, base, previous->y, previous->x);

        /*redraw the destination tile if player moves*/
        if (current->x != previous ->x ||
            current->y != previous ->y )
        {
                redraw_tile(model, base, current->y, current->x);
        }
    }
}
/*----- Function: redraw_board_changes -----
* PURPOSE: Redraws tiles which have been changed between current 
*          and previous models
*
* INPUT:   current  - Current game model
*          previous - Previous game model
*          base     - Start of the game buffer to draw into
*
* OUTPUT:  Updates the screen buffer with changed tiles
*
* ASSUMPTIONS: both models contain boards that are the same dimension
*/
static void redraw_board_changes(const Model *current, const Model *previous,
                                UINT8 *base){
    int row, col;

    /*Check tiles for changes*/
    for (row = 0; row < GRID_HEIGHT; row++)
    {
        for (col = 0; col < GRID_WIDTH; col++)
        {
            /*Redraw based on tile type/wall changes*/ 
            if (current->grid[row][col].type !=
                previous->grid[row][col].type ||
                current->grid[row][col].walls !=
                previous->grid[row][col].walls)
            {
                redraw_tile(current, base, row, col);
            }
        }
    }
}
/*----- Function: render_changes -----
* PURPOSE: Redraws only parts of the game board that have been changed between current
*          and previous models.  
*
* INPUT:   current  - Current game model
*          previous - Previous model for comparing
*          base     - Start of the game buffer to draw into
*
* OUTPUT:  Updates the screen buffer
*
*/
void render_changes(const Model *current, const Model *previous, UINT8 *base){
    /*Redraw entire board if level changes*/
    if (current->level_select != previous->level_select)
    {
        render(current, base);
        return;
    }

    /*Redraw changes on tiles, walls, player, and ally*/ 
    redraw_board_changes(current, previous, base);
    if (animate(current) != animate(previous)) {
        redraw_alts(current, base);
    }

    redraw_player_changes(current, &current->player,
                         &previous->player, base, current->player_blocked, previous->player_blocked);

    redraw_player_changes(current, &current->ally,
                         &previous->ally, base, current->ally_blocked, previous->ally_blocked);
}

/*----- Function: render_title -----
* PURPOSE: Draws the title "TiLEZ" to the screen and the level # as well
*
* INPUT:   model - the game state to draw, used for level #
*          base  - start of the frame buffer to draw into
*
* OUTPUT:  None
*
* ASSUMPTIONS: 
*/
static void render_title(const Model *model, UINT8 *base) {
    char level_txt[] = "LEVEL ?";

    level_txt[6] = '0' + model->level_select + 1; /* replacing the ? with the level number*/
    plot_string(base, TITLE_ROW, TEXT_COL, "TILEZ");
    plot_string(base, LEVEL_ROW, TEXT_COL, level_txt);
}



/*----- Function: render -----
* PURPOSE: Draws one full frame of the game from the model: clears the
*          screen, then draws the board, the player and the ally. Also,
*          draws the game name and level #
*
* INPUT:   model - the game state to draw
*          base  - start of the frame buffer to draw into
*
* OUTPUT:  None
*
* ASSUMPTIONS: 
*/
void render(const Model *model, UINT8 *base)
{
    UINT8 anim_state = animate(model);
    clear_screen((UINT32 *)base);

    render_board(model->grid, base, anim_state);
    render_title(model, base);
    render_player(&model->player, base, model->player_blocked, anim_state);
    render_ally(&model->ally, base, model->ally_blocked, anim_state);
}
