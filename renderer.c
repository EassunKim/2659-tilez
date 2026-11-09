
#include "renderer.h"
#include "raster.h"
#include "types.h"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

void render_tile(const Tile *tile, UINT8 *base, int row, int col)
{
    /* Do not draw void tiles */
    if (tile->type == T_VOID)
    {
        return;
    }

    /* Draw the outline of the tile */
    plot_rectangle((UINT32 *)base, row, col, TILE_HEIGHT, TILE_WIDTH);

    /* Draw a fire tile */
    if (tile->type == T_FIRE)
    {
        plot_triangle((UINT32 *)base, row + 8, col + 10, 12, 16, 0);
    }

    /* Draw a goal tile */
    else if (tile->type == T_GOAL)
    {
        plot_square((UINT32 *)base, row + 8, col + 8, 16);
    }

    /* Draw a slip tile */
    else if (tile->type == T_SLIP)
    {
        plot_line((UINT32 *)base, row + 8, col + 8,
                  row + 16, col + 16);

        plot_line((UINT32 *)base, row + 16, col + 16,
                  row + 8, col + 24);
    }

    render_walls(tile, base, row, col);

    /* Normal tiles only have the outline */
}


void render_walls(const Tile *tile, UINT8 *base, int row, int col)
{
    if (tile->walls & WALL_NORTH)
    {
        plot_horizontal_line((UINT32 *)base, row, col, TILE_WIDTH);
    }

    if (tile->walls & WALL_EAST)
    {
        plot_vertical_line((UINT32 *)base, row,
                           col + TILE_WIDTH - 1, TILE_HEIGHT);
    }

    if (tile->walls & WALL_SOUTH)
    {
        plot_horizontal_line((UINT32 *)base,
                             row + TILE_HEIGHT - 1,
                             col, TILE_WIDTH);
    }

    if (tile->walls & WALL_WEST)
    {
        plot_vertical_line((UINT32 *)base, row, col, TILE_HEIGHT);
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

    plot_square((UINT32 *)base, row + 8, col + 8, 16);

    if (player->facing == NORTH)
    {
        plot_vertical_line((UINT32 *)base, row + 4, col + 16, 5);
    }
    else if (player->facing == EAST)
    {
        plot_horizontal_line((UINT32 *)base, row + 16, col + 23, 5);
    }
    else if (player->facing == SOUTH)
    {
        plot_vertical_line((UINT32 *)base, row + 23, col + 16, 5);
    }
    else if (player->facing == WEST)
    {
        plot_horizontal_line((UINT32 *)base, row + 16, col + 4, 5);
    }
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
*              The player and ally are drawn the same way for now.
*/

/*redraw_tile - clears and redraws a single tile and any players on it*/

static void redraw_tile(const Model *model, UINT8 *base, int row, int col)
{
    int screen_row = 40 + row * TILE_HEIGHT;
    int screen_col = 160 + col * TILE_WIDTH;

    clear_region((UINT32 *)base, screen_row, 
                screen_col, TILE_HEIGHT, TILE_WIDTH);

    render_tile(&model->grid[row][col], base,
                screen_row, screen_col);

    if (model->player.x == col && model->player.y == row)
    {
        render_player(&model->player, base);
    }

    if (model->ally.x == col && model->ally.y == row)
    {
        render_player(&model->ally, base);
    }
    

}
/*Player_changed - checks if a player position or direction changed*/
static int player_changed(const Player *current, const Player *previous)
{
    return current->x != previous->x || current->y != previous->y ||
            current->facing != previous->facing;
}

/*redraw_player_changes - redraws tiles affect by player movement or rotation*/
static void redraw_player_changes(const Model *model, const Player *current, 
                                const Player *previous, UINT8 *base)
{
    if (player_changed(current, previous))
    {
        redraw_tile(model, base, previous->y, previous->x);

        if (current->x != previous ->x ||
            current->y != previous ->y )
        {
                redraw_tile(model, base, current->y, current->x);
        }
    }
}
/*finds and redraws changed tiles*/
static void redraw_board_changes(const Model *current, const Model *previous,
                                UINT8 *base)
{
    int row, col;

    for (row = 0; row < GRID_HEIGHT; row++)
    {
        for (col = 0; col < GRID_WIDTH; col++)
        {
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
/*Updates only changed parts of the screen*/
void render_changes(const Model *current, const Model *previous, UINT8 *base)
{
    if (current->level_select != previous->level_select)
    {
        render(current, base);
        return;
    }

    redraw_board_changes(current, previous, base);

    redraw_player_changes(current, &current->player,
                         &previous->player, base);

    redraw_player_changes(current, &current->ally,
                         &previous->ally, base);
}
void render(const Model *model, UINT8 *base)
{
    clear_screen((UINT32 *)base);

    render_board(model->grid, base);
    render_player(&model->player, base);
    render_player(&model->ally, base);
}
