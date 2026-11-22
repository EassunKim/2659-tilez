# TileZ

## Authors

- Matthew Kim
- Mohamad Hamade (Withdrew from course after work on checkpoint 2)
- Ujjwal Bhandari

## Checkpoint 3 - November 10
Our game is now in a playable state. The WASD commands work to move the character and the screen is updated on tick as intended. At this point we have 'level 0' in the game as a way of testing out every tile type and movement. Proper puzzle levels will be added as we go through and the main game loop already loops through the levels, though there is only one at the moment. 

### Usage 
```asm
make all
tilez.c
```

### Ujjwal Bhandari (Renderer.c/h, tilez.c/h)

| File                        | Description                                                                                             |
| --- | --- |
| `Renderer.c  / renderer.h`  | The renderer was implemented without Minimized Screen Plotting in CP2, it is implemented now. Using checks to confirm change, and previously designed raster functions to prevent redrawing the entire board. |
| `Tilez.c /  Tilez.h`        | Implemented double buffering within Tilez.c, where screen buffers are alternated. Which reduces flickering and improves rendering speed.|

### Matthew Kim (Tilez.c, input.c/h)
| File                        | Description                                                                                             |
| --------------------------- | ------------------------------------------------------------------------------------------------------- |
| `Tiles.c`                | Implemented the main game loop, as well as timing functions to make input module work|
| `input.c/h`                  | Created the input module. Will take in an input, save it to the model and update the screen based on it on the next tick |
| Other Fixes | Changed renderer to use new bitmaps in bitmaps.c to replace placeholders from checkpoint2  |

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
