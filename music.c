#include "music.h"
#include "psg.h"

#define CHANNEL 1 /* Our song takes channel A, as specified */
#define VOLUME 6

/* on/off for the registers */
#define ON 1 
#define OFF 0

/* notes used - referenced music note doc on d2l*/
#define NOTE_C4 261
#define NOTE_E4 329
#define NOTE_G4 391
#define NOTE_C5 523
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
    {NOTE_C4, QUARTER},
    {NOTE_E4, QUARTER},
    {NOTE_G4, QUARTER},
    {NOTE_C5, HALF},
    {NOTE_G4, QUARTER},
    {NOTE_E4, QUARTER}
};

/* trackers for the song used by both functions*/
static UINT8 cur_note;
static UINT32 time;

/* taking the total song size and dividing by the size of a note*/
#define SONG_LENGTH (sizeof(main_theme) / sizeof(main_theme[0])) 


void start_music(void) {
    /* reset trackers */
    cur_note = 0;
    time = 0;

    /*start at main theme first note*/
    set_tone(CHANNEL, main_theme[0].tuning);
    enable_channel(CHANNEL, ON, OFF);    
    set_volume(CHANNEL, VOLUME);
}

void update_music(UINT32 ticks) {
    /* how long has it been, and check if we need to update the note*/
    time += ticks;
    if (time >= main_theme[cur_note].duration) {
        time = 0;
        cur_note += 1;
        if (cur_note >= SONG_LENGTH) {
            cur_note = 0;
        }
        set_tone(CHANNEL, main_theme[cur_note].tuning);
    }
}