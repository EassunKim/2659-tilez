/* copy past of model tester, using renderer*/

#include <stdio.h>
#include <osbind.h>
#include "model.h"
#include "events.h"
#include "renderer.h"
#include "raster.h"

#define LABEL_ROW   8
#define RESULT_ROW  24
#define TEXT_COL    8
#define KEY_WIDTH   16   /* pixels for the key and a space before the label */

static int passed = 0;
static int failed = 0;

/* A copy of the route in tst_modl.c. Each step is a key, the expected state
   after it, and what the step tests. Keep the two copies matching. */
typedef struct {
    char key;
    UINT8 px, py, pf;
    UINT8 ax, ay, af;
    UINT16 count;
    BOOL complete;
    const char *test_label;
} Route;

static const Route Route0[] = {
    /*test all inputs*/
    {'W', 3,3,NORTH, 4,3,NORTH,  1, FALSE, "both step, player on fire"},
    {'A', 3,3,WEST , 4,3,WEST ,  1, FALSE, "rotate left, left wraps N to W"},
    {'D', 3,3,NORTH, 4,3,NORTH,  1, FALSE, "rotate right, right wraps W to N"},
    {'S', 3,3,SOUTH, 4,3,SOUTH,  1, FALSE, "turn around"},
    /*collisions*/
    {'W', 3,4,SOUTH, 4,4,SOUTH,  2, FALSE, ""},
    {'W', 3,4,SOUTH, 4,5,SOUTH,  3, FALSE, "wall collision and slide blocking"},
    {'W', 3,4,SOUTH, 4,5,SOUTH,  3, FALSE, "both blocked"},
    {'D', 3,4,WEST , 4,5,WEST ,  3, FALSE, ""},
    {'W', 2,4,WEST , 4,5,WEST ,  4, FALSE, "ally blocked, player moves"},
    {'W', 2,4,WEST , 4,5,WEST ,  4, FALSE, "test void"},
    /*move to next case (slip tiles on right edge)*/
    {'D', 2,4,NORTH , 4,5,NORTH ,  4, FALSE, ""},
    {'W', 2,3,NORTH , 4,4,NORTH ,  5, FALSE, ""},
    {'D', 2,3,EAST , 4,4,EAST ,  5, FALSE, ""},
    {'W', 3,3,EAST , 5,4,EAST ,  6, FALSE, ""},
    {'W', 4,3,EAST , 7,4,EAST ,  7, FALSE, "ally slides, stops on 2nd slip"},
    /*direct ally to goal, uses a quick fire tile reset*/
    {'A', 4,3,NORTH , 7,4,NORTH ,  7, FALSE, ""},
    {'W', 4,2,NORTH , 7,3,NORTH ,  8, FALSE, ""},
    {'A', 4,2,WEST , 7,3,WEST ,  8, FALSE, ""},
    {'W', 3,3,NORTH , 4,3,NORTH ,  9, FALSE, "player on fire, reset"},
    {'S', 3,3,SOUTH , 4,3,SOUTH , 9, FALSE, ""},
    {'W', 3,4,SOUTH , 4,4,SOUTH , 10, FALSE, ""},
    {'W', 3,4,SOUTH , 4,5,SOUTH , 11, FALSE, "ally slide blocked by void"},
    {'A', 3,4,EAST , 4,5,EAST , 11, FALSE, ""},
    {'W', 4,4,EAST , 5,5,EAST , 12, TRUE, "ally on goal, level complete"}
};

/*----- Function: check_step -----
* PURPOSE: compares the model to one route step and counts a pass or fail
*
* INPUT:   model - the model to check
*          step  - the expected state
*
* OUTPUT:  TRUE if the model matches the step

* From Model: unchanged
*/
static BOOL check_step(const Model *model, const Route *step) {
    if (model->player.x == step->px && model->player.y == step->py &&
        model->player.facing == step->pf &&
        model->ally.x == step->ax && model->ally.y == step->ay &&
        model->ally.facing == step->af &&
        model->move_count == step->count &&
        model->level_complete == step->complete) {
        passed++;
        return TRUE;
    }

    failed++;
    return FALSE;
}

/*----- Function: apply_key -----
* PURPOSE: applies the route key to the model
*
* INPUT:   model - the model to update
*          key   - W, A, D or S
*
* From Model: unchanged
*/
static void apply_key(Model *model, char key) {
    switch (key) {
        case 'W': asynch_forward(model);
        break;
        case 'A': asynch_rotate_left(model);
        break;
        case 'D': asynch_rotate_right(model);
        break;
        case 'S': asynch_turn_around(model);
        break;
    }
    synch_req(model);
    cond_check(model);
}

/*----- Function: show_frame -----
* PURPOSE: draws one frame with its text above the board, waits for a key,
*          then clears the screen
*
* INPUT:   model  - the model to draw
*          base   - the frame buffer
*          key    - the key just applied, or ' ' for the first frame
*          label  - what this frame tests
*          result - "PASS", "FAIL" or "" for no result

* From Model: takes place of print_model
*/
static void show_frame(const Model *model, UINT8 *base, char key, const char *label, const char *result) {
    render(model, base);
    plot_character(base, LABEL_ROW, TEXT_COL, key);
    plot_string(base, LABEL_ROW, TEXT_COL + KEY_WIDTH, (char *)label);
    plot_string(base, RESULT_ROW, TEXT_COL, (char *)result);
    Cnecin();
    clear_screen((UINT32 *)base);
}

/*----- Function: run_route -----
* PURPOSE: plays a route, drawing and checking the model after every step
*
* INPUT:   model - the model to play on, already loaded
*          base  - the frame buffer
*          route - the steps to play
*          count - how many steps are in the route
*
** From Model: now uses show_frame for each move
*/
static void run_route(Model *model, UINT8 *base, const Route *route, int count) {
    int i;
    const char *result;

    for (i = 0; i < count; i++) {
        apply_key(model, route[i].key);
        result = check_step(model, &route[i]) ? "PASS" : "FAIL";
        show_frame(model, base, route[i].key, route[i].test_label, result);
    }
}

/*----- Function: main -----
* PURPOSE: hides the text cursor, draws the spawn frame, plays the route,
*          then restores the cursor and prints the totals
*/
int main(void) {
    Model model;
    UINT8 *base = (UINT8 *)Physbase(); /*get frame buffer*/

    /* clear screen*/
    printf("\033f");
    clear_screen((UINT32 *)base);

    model_load_level(&model, 0);
    show_frame(&model, base, ' ', "level 0", "");

    run_route(&model, base, Route0, sizeof(Route0) / sizeof(Route0[0]));

    printf("\033e");
    printf("%d passed, %d failed\n", passed, failed);
    Cnecin();
    return 0;
}
