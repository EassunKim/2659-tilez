#include "effects.h"
#include "types.h"
#include "psg.h"

#define CHANNEL_C 2 /* channel C will be used for effects */
#define ENABLE_ENVELOPE 0x10 

#define ON 1
#define OFF 0

/* Envelope Shapes */
#define DECAY 0x00

/* Tuning and Sustain */
#define MOVE_TUNING 100
#define MOVE_SUSTAIN 0x0300

#define FIRE_NOISE 30 /* using noise, to use noise, and to get a deep sound*/
#define FIRE_SUSTAIN 0x1800

void play_move(void) {
    set_tone(CHANNEL_C,MOVE_TUNING);
    enable_channel(CHANNEL_C, ON, OFF);
    set_volume(CHANNEL_C,ENABLE_ENVELOPE);
    set_envelope(DECAY, MOVE_SUSTAIN);
}

void play_fire(void) {
    set_noise(FIRE_NOISE);
    enable_channel(CHANNEL_C, OFF, ON);
    set_volume(CHANNEL_C,ENABLE_ENVELOPE);
    set_envelope(DECAY, FIRE_SUSTAIN);
}