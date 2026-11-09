
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
    /* one case for each wall */
    if (tile->walls & WALL_NORTH) {
    plot_32bit_bitmap((UINT32 *)base, row - WALL_OFFSET, col, wall_horizontal, WALL_ATSU);
    }

    if (tile->walls & WALL_SOUTH) {
    plot_32bit_bitmap((UINT32 *)base, row + TILE_HEIGHT - WALL_OFFSET, col, wall_horizontal, WALL_ATSU);
    }
    
    if (tile->walls & WALL_EAST) {
    plot_32bit_bitmap((UINT32 *)base, row - WALL_OFFSET, col + TILE_WIDTH - WALL_OFFSET, wall_vertical, WALL_LENGTH);
    }

    if (tile->walls & WALL_WEST) {
    plot_32bit_bitmap((UINT32 *)base, row - WALL_OFFSET, col - WALL_OFFSET, wall_vertical, WALL_LENGTH);
    }

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



/*----- Function: render -----
* PURPOSE: Draws one full frame of the game from the model: clears the
*          screen, then draws the board, the player and the ally.
*
* INPUT:   model - the game state to draw
*          base  - start of the frame buffer to draw into
*
* OUTPUT:  None
*
* ASSUMPTIONS: Redraws everything each call (no minimised plotting yet).
*/
void render(const Model *model, UINT8 *base)
{
    clear_screen((UINT32 *)base);

    render_board(model->grid, base);
    render_player(&model->player, base);
    render_ally(&model->ally, base);
}
