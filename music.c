#include "music.h"
#include "psg.h"

#define CHANNEL 0 /* Our song takes channel A, as specified */
#define VOLUME 10

/* notes used - referenced music note doc on d2l*/
#define MID_C 261

/* timings - arbitrary tempo, but using idea of quarter note, eighth etc. */
#define QUARTER 16
#define EIGHTH 8
#define SIXTEENTH 4
#define HALF 32

/* Note Struct */
typedef struct {
    UINT16 tuning;
    UINT8 duration;
} Note;

/* Main theme */
static const Note main_theme[] = {
    {MID_C, QUARTER}
};

/* trackers for the song used by both functions*/
static UINT8 cur_note;
static UINT32 time;

/* taking the total song size and dividing by the size of a note*/
#define SONG_LENGTH (sizeof(main_theme) / sizeof(main_theme[0])) 
void start_music(void) {

}

void update_music(UINT32 ticks) {

}