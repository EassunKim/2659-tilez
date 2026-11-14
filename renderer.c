
#include "renderer.h"
#include "raster.h"
#include "types.h"

#include "bitmaps.h"


void render_tile(const Tile *tile, UINT8 *base, int row, int col)
{
    plot_32bit_bitmap((UINT32 *)base, row, col, tile_bitmaps[tile->type], TILE_HEIGHT);


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
void render_board(const Tile board[GRID_HEIGHT][GRID_WIDTH],
                  UINT8 *base)
{
    int row, col;

    for (row = 0; row < GRID_HEIGHT; row++)
    {
        for (col = 0; col < GRID_WIDTH; col++)
        {
            render_tile(&board[row][col], base,
                        40 + row * TILE_HEIGHT,
                        160 + col * TILE_WIDTH);
        }
    }
}


void render_player(const Player *player, UINT8 *base)
{
    int row, col;

    row = 40 + player->y * TILE_HEIGHT;
    col = 160 + player->x * TILE_WIDTH;

    plot_16bit_bitmap((UINT16 *)base, row+ CHAR_OFFSET, col+ CHAR_OFFSET, player_bitmaps[player->facing], CHAR_HEIGHT);
}

void render_ally(const Player *player, UINT8 *base)
{
    int row, col;

    row = 40 + player->y * TILE_HEIGHT;
    col = 160 + player->x * TILE_WIDTH;

    plot_16bit_bitmap((UINT16 *)base, row + CHAR_OFFSET, col+ CHAR_OFFSET, ally_bitmaps[player->facing], CHAR_HEIGHT);
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

    /*Clear the previous tile*/
    clear_region((UINT32 *)base, screen_row, 
                screen_col, TILE_HEIGHT, TILE_WIDTH);

    /*Restore a tile*/
    render_tile(&model->grid[row][col], base,
                screen_row, screen_col);

    /*Redraw player/ally if they are present*/
    if (model->player.x == col && model->player.y == row)
    {
        render_player(&model->player, base);
    }

    if (model->ally.x == col && model->ally.y == row)
    {
        render_ally(&model->ally, base);
    }
}
/*----- Function: redraw_player_changes -----
* PURPOSE: Redraws tiles when a player/ally moves or changes direction
*          
*
* INPUT:   model    - Current game model
*          current  - Current player/ally state
*          previous - Previous player/ally state
*          base     - Start of the game buffer to draw into
*
*
* OUTPUT:  Updates the screen buffer with changed tiles
*
*/
static void redraw_player_changes(const Model *model, const Player *current, 
                                const Player *previous, UINT8 *base) {
    /*Player movement/direction change check*/
    if (current->x != previous->x || current->y != previous->y ||
            current->facing != previous->facing)
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

    redraw_player_changes(current, &current->player,
                         &previous->player, base);

    redraw_player_changes(current, &current->ally,
                         &previous->ally, base);
}


/*----- Function: render -----
* PURPOSE: Draws one full frame of the game from the model: clears the
*          screen, then draws the board, the player and the ally.
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
    clear_screen((UINT32 *)base);

    render_board(model->grid, base);
    render_player(&model->player, base);
    render_ally(&model->ally, base);
}
