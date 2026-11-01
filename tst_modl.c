#include <stdio.h>
#include "model.h"
#include "events.h"

static int passed = 0;
static int failed = 0;

/* This new tester functions by making the player run a route. Test level 0 is currently laid out in a way
that I believed would allowa me to run a simple "route" to test all the behaviors I've put into the model thus 
far
*/
/* struct used for each 'step' of a route */
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
    {'W', 4,3,EAST , 7,4,EAST ,  7, FALSE, "ally slips and is blocked by void"},
    /*direct ally to goal, uses a quick fire tile reset*/
    {'A', 4,3,NORTH , 7,4,NORTH ,  7, FALSE, ""},
    {'W', 4,2,NORTH , 7,3,NORTH ,  8, FALSE, ""},
    {'A', 4,2,WEST , 7,3,WEST ,  8, FALSE, ""},
    {'W', 3,3,NORTH , 4,3,NORTH ,  9, FALSE, ""},
    {'S', 3,3,SOUTH , 4,3,SOUTH , 9, FALSE, ""},
    {'W', 3,4,SOUTH , 4,4,SOUTH , 10, FALSE, ""},
    {'W', 3,4,SOUTH , 4,5,SOUTH , 11, FALSE, ""},
    {'A', 3,4,EAST , 4,5,EAST , 11, FALSE, ""},
    {'W', 4,4,EAST , 5,5,EAST , 12, TRUE, "ally on goal, level complete"},
};

/*used with the struct to do the needed inputs*/
static char dir_char(UINT8 facing) {
    switch (facing) {
        case NORTH: return 'N';
        case EAST:  return 'E';
        case SOUTH: return 'S';
        case WEST:  return 'W';
        default:    return '?';
    }
}

/*----- Function: wait_key -----
* PURPOSE: pauses screen
static void wait_key(void) {
    printf("-- press any key --\n");
    getchar();
}

/*----- Function: print_model -----
* PURPOSE: prints every model property except the grid
*
* INPUT:   model - the model to print
*/
static void print_model(const Model *model) {
    printf("  P(%d,%d,%c) A(%d,%d,%c) c=%u win=%d time=%u.%d\n",
           model->player.x, model->player.y, dir_char(model->player.facing),
           model->ally.x, model->ally.y, dir_char(model->ally.facing),
           model->move_count, model->level_complete,
           model->seconds, model->tick_count);
    printf("  spawns P(%d,%d,%c) A(%d,%d,%c) level=%d\n",
           model->player_spawn.x, model->player_spawn.y,
           dir_char(model->player_spawn.facing),
           model->ally_spawn.x, model->ally_spawn.y,
           dir_char(model->ally_spawn.facing), model->level_select);
}

/*----- Function: check_value -----
* PURPOSE: compares one value to its expected value and counts a pass or fail
*
* INPUT:   name     - what is being checked
*          actual   - the value the code produced
*          expected - the value it should be
*/
static void check_value(const char *name, int actual, int expected) {
    if (actual == expected) {
        passed++;
        printf("  PASS %s\n", name);
    } else {
        failed++;
        printf("  FAIL %s: got %d, expected %d\n", name, actual, expected);
    }
}

/*----- Function: check_step -----
* PURPOSE: compares the model to one route step and counts a pass or fail
*
* INPUT:   model - the model to check
*          step  - the expected state
*/
static void check_step(const Model *model, const Route *step) {
    if (model->player.x == step->px && model->player.y == step->py &&
        model->player.facing == step->pf &&
        model->ally.x == step->ax && model->ally.y == step->ay &&
        model->ally.facing == step->af &&
        model->move_count == step->count &&
        model->level_complete == step->complete) {
        passed++;
        printf("  PASS\n");
    } else {
        failed++;
        printf("  FAIL expected P(%d,%d,%c) A(%d,%d,%c) c=%u win=%d\n",
               step->px, step->py, dir_char(step->pf),
               step->ax, step->ay, dir_char(step->af),
               step->count, step->complete);
    }
}

/*----- Function: apply_key -----
* PURPOSE: applies the route key to the model
*
* INPUT:   model - the model to update
*          key   - W, A, D or S
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
}

/*----- Function: run_route -----
* PURPOSE: plays a route
*
* INPUT:   model - the model to play on, already loaded
*          route - the steps to play
*          count - how many steps are in the route
*/
static void run_route(Model *model, const Route *route, int count) {
    int i;

    for (i = 0; i < count; i++) {
        apply_key(model, route[i].key);
        printf("%c %s\n", route[i].key, route[i].test_label);
        print_model(model);
        check_step(model, &route[i]);

        /*added in due to screen size limitation of gu*/
        if ((i + 1) % 5 == 0) {
            wait_key();
        }
    }
    wait_key();
}

/*----- Function: test_level0 -----
* PURPOSE: plays the level 0 route
*/
static void test_level0(Model *model) {
    model_load_level(model, 0);

    run_route(model, Route0,
              sizeof(Route0) / sizeof(Route0[0]));

    wait_key();
}


int main(void) {
    Model model;

    test_level0(&model);

    printf("\n%d passed, %d failed\n", passed, failed);
}
