#include "splash.h"
#include "raster.h"
#include "input.h"
#include "splashbg.h"

/* boxes selection coordinates: row, col, width, height*/
#define BOX_HEIGHT 40
#define BOX_WIDTH 110
#define BOX_ROW 195
#define PLAY_COL 210
#define QUIT_COL 335

#define ENTER 0x0D

static void draw_menu(UINT8 *base, UINT8 select) {
    UINT16 i;
    int col;

    /*Draw base screen*/
    for (i = 0; i < SPLASH_LEN; i++) {
        ((UINT32 *)base)[i] = splash_image[i];
    } 

    if (select == SPLASH_PLAY) { 
        col = PLAY_COL;
    } else if (select == SPLASH_QUIT) {
        col = QUIT_COL;
    }
    plot_rectangle((UINT32 *)base, BOX_ROW, col, BOX_HEIGHT, BOX_WIDTH);
     
}

UINT8 splash_screen(UINT8 *base) {
    UINT8 select = SPLASH_PLAY;
    char key;

    draw_menu(base, select);

    while(TRUE) { /* loop until game is quit or started by returning */
        if(has_input()) {
            key = get_input();
            switch(key) {
                case 'a': case 'A': /* I am doing two switches here to simplify adding a third option for 2p later*/
                    select = 1 - select;
                    draw_menu(base,select);
                    break;
                case 'd': case 'D':
                    select = 1 - select;
                    draw_menu(base,select);
                    break;
                case ENTER:
                    return select;
            }
        }
    }
}
