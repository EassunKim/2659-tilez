# TileZ

## Authors

- Matthew Kim
- Mohamad Hamade (Withdrew from course after work on checkpoint 2)
- Ujjwal Bhandari

## How to Play / Test
All files can be made via our MakeFile. Running `make all` will make all our testers (tst_*.tos) and the main game (tilez.tos) that can be run through gulam or the STEEM desktop. To make specific files please see documentation at the top of MAKEFILE

## Checkpoint 4 - November 25 (use of class allotted 2 day extension)

### Files Completed

#### `psg.h` / `psg.c` - written by Ujjwal

A functioning psg module used in both the game's main theme loop and in its sound effects.

#### `tst_psg.c` - written by Ujjwal

tester for Ujjwal's psg function. Goes through all functions in psg and confirms the register is in the correct state

#### `music.c` / `music.h` - written by Matthew

The two files as specified by the checkpoint 4 pdf. They utilize the psg functions to implement `start_music` and `update_music`.

#### `tst_mus.c` - written by Matthew

A lightweight tester that simply plays our song. Can be used by running these two commands in `gu`:

```sh
make tst_mus.tos
tst_music.tos
```

#### `effects.h` / `effects.c` - written by Matthew

The sound effect files. For the submission of this checkpoint we chose to include one sound effect for coming in contact with a fire tile and one for the player moving.

#### `splash.h` / `splash.c` / `splashbg.c` / `splashbg.h` - written by Matthew

functions to draw the splash screen into the frame buffer. The bitmap is contained within splashbg.c. Splash.c will draw the map to the screen and draw a square using plot_rectangle() to highlight the options. Upon pressing A or D, the highlight will move simulating menu movement

#### `tilez.c`, `cond.c`, `asynch.c` additions - written by Matthew Kim

- Added the main song loop into the main game loop in tilez.c and added the sound effects to their respective events.
- Added play_fire and play_move to their respective events. Functioning in the main game.
- Moved main game loop from main into play() to clean up adding the splash screen prior to gameplay
- The original bitmap for splashbg was 32000 items which caused a memory error, AI was used to compress the input

### Other Fixes and notes

#### Fixes

- Added in short animation loops for our sprites. Using the clock, the event tiles and player/ally update every half second or every 35/70 ticks.
- Added in printing to the main game screen. The game title and the current level are now printed above and below the level grid respectively.
- Changed the sprite for the slip tile. The old one was a right facing arrow that was causing confusion when encountered any way that wasn't from the right. Replaced with my best effort at an "icy tile".

Did a comment check over/fix 
- Added proper headers to `types.h`, `tile.c/.h`, `player.c/.h`, `levels.h`, `synch.c/.h`, `music.c`, `model.c/.h`, `events.h`, `cond.c/.h`, `asynch.c/.h`
- Moved comments to header file in `player.h` from `player.c`

#### Notes

- The current main theme is a place holder
- There is currently only one level but the system is in place to add more.
- music.c works by taking over channel A of the psg and playing notes for set duration. The key click sound on Steem was interfering with this. For this checkpoint we chose, after confirming with our professor, that disabling the sound tick all together was the simplest solution.

## Checkpoint 3 - November 10

Our game is now in a playable state. The WASD commands work to move the character and the screen is updated on tick as intended. At this point we have 'level 0' in the game as a way of testing out every tile type and movement. Proper puzzle levels will be added as we go through and the main game loop already loops through the levels, though there is only one at the moment.

### Usage

```asm
make all
tilez.c
```

### Ujjwal Bhandari (Renderer.c/h, tilez.c/h)

| File                       | Description                                                                                                                                                                                                   |
| -------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `Renderer.c  / renderer.h` | The renderer was implemented without Minimized Screen Plotting in CP2, it is implemented now. Using checks to confirm change, and previously designed raster functions to prevent redrawing the entire board. |
| `Tilez.c /  Tilez.h`       | Implemented double buffering within Tilez.c, where screen buffers are alternated. Which reduces flickering and improves rendering speed.                                                                      |

### Matthew Kim (Tilez.c, input.c/h)

| File | Description |

| --------------------------- | ------------------------------------------------------------------------------------------------------- |
| `Tiles.c` | Implemented the main game loop, as well as timing functions to make input module work|
| `input.c/h` | Created the input module. Will take in an input, save it to the model and update the screen based on it on the next tick |
| Other Fixes | Changed renderer to use new bitmaps in bitmaps.c to replace placeholders from checkpoint2 |

## Checkpoint 2 - October 19

Model, raster graphics library, and renderer.

### testing

Testing is currently done through the make file. Running the bellow commands will create a .tos for each
of our testers.

```asm
make               builds the model test driver, tst_modl.tos
make tst_rast.tos  builds the raster library test driver
make rendtest.tos  builds the renderer unit test
make tst_rend.tos  builds the renderer test driver (draws on screen)
make all           builds all four
```

### Mohamad Hamade (raster graphics and renderer)

| File                        | Description                                                                                             |
| --------------------------- | ------------------------------------------------------------------------------------------------------- |
| `rast_asm.s`                | Assembly raster library with 9 functions for clearing the screen and drawing pixels, lines, and shapes. |
| `raster.h`                  | Header file declaring all 14 raster graphics functions.                                                 |
| `renderer.c` / `renderer.h` | Renderer functions for drawing tiles, walls, the game board, and player.                                |

### Ujjwal Bhandari ()

| File       | Description                                                                                                                                                                                     |
| ---------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Rast_asm.s | Raster library containing many methods to plot pixels, lines, shapes, bitmaps, and characters. My focus was on implementing the bitmap functions along with single characters and strings.      |
| TEST.c     | Contains multiple test cases for each raster function. Bitmap/Char tests are based around boundary handling, input handling, and base cases. Verifies expected results and reports a pass/fail. |

### Matthew Kim (model and events)

| File                    | Description                                                                                                                                                                                                                          |
| ----------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `types.h`               | Types headers and declarations used for model, includes signed/unsigned int8/16/32 and a boolean type                                                                                                                                |
| `player.h` / `player.c` | Player behavior files. Used to outline the behaviors of both player and ally objects. NOTE: currently and for the sake of time ally and player are drawn in the same way, this will be fixed when animating them both in checkpoint3 |
| `tile.h` / `tile.c`     | Tile behavior files. Includes the way in which tiles are differentiated and how to determine when walls are present                                                                                                                  |
| `levels.h` / `levels.c` | Level behavior files. Uses tile and player to outline our grid based levels, as well as how we will be storing level designs                                                                                                         |
| `model.h` / `model.c`   | Model files as outlined in checkpoint 2, uses all files above. Creates a model holding objects and their values as well as functions that will be used by events (move forward, turn, etc,)                                          |
| `events.h`              | Header file for next 3 anvent files. From d2l                                                                                                                                                                                        |
| `asynch.h` / `asynch.c` | Handler for asynchronous events like movement and rotation                                                                                                                                                                           |
| `synch.h` / `synch.c`   | Handler for synchronous events. As I understand it, we do not currently have any synchronous events reliant on the clock. We will include them as we animate but for not it is a file with a dead function in it.                    |
| `cond.h` / `cond.c`     | Checks conditional events like intersecting a special tile                                                                                                                                                                           |
| `tst_modl.c`            | Tester for my model. Includes a check function that will essentially check the values in the model and return a pass or fail after one of our events is called.                                                                      |
| `tst_rast.c`            | Similar to the above tester. Print replaced with render() from renderer.c but still returns a pass or fail and the totals                                                                                                            |
| `MAKEFILE`              | Our make file. AI (Gemini) was prompted to teach me how to make a 3 way make file (since we have 3 testers) as I had never seen it. An example was provided but code is all my own                                                   |
