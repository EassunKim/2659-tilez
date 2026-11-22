#include <stddef.h>
#include <osbind.h>
#include "psg.h"

BOOL read_psg(UINT8 reg, UINT8 *value) {
    volatile UINT8 *PSG_reg_select = (volatile UINT8 *)PSG_SELECT;
    long old_ssp;

    /* validate values */
    if (reg >= REG_COUNT || value == NULL) {
        return FALSE;
    }

    old_ssp = Super(0);
    *PSG_reg_select = reg;
    *value = *PSG_reg_select;

    Super(old_ssp);
    return TRUE;
}

void write_psg(UINT8 reg, UINT8 val) {
    volatile UINT8 *PSG_reg_write = (volatile UINT8 *)PSG_WRITE;
    volatile UINT8 *PSG_reg_select = (volatile UINT8 *)PSG_SELECT;
    long old_ssp;

    /* validate values */
    if (reg >= REG_COUNT || val < 0 || val > 255) {
        return;
    }

    old_ssp = Super(0);
    *PSG_reg_select = reg;
    *PSG_reg_write = val;

    Super(old_ssp);
    return;
}

void set_tone(UINT8 channel, UINT16 tuning) {
    /* Validate values */
    if (channel > CHANNEL_C || tuning > MAX_TONE) {
        return;
    }
    
    /* will take the channel #, multiplies by 2 to reach the correct tone register and sets it.
    Two writes are used to transfer both the fine and coarse tone. A shift is used to isolate the top
    4 bits*/
    write_psg(channel * 2, (UINT8)(tuning & 0xFF));
    write_psg(channel * 2 + 1, (UINT8)(tuning >> 8));

}

void set_volume(UINT8 channel, UINT8 volume) {
    /* Validate values */
    if (channel > CHANNEL_C || volume > MAX_VOLUME) {
        return;
    }
    
    /* will take the channel # and add 8 to reach the volume reigster for each channel*/
    write_psg(channel + VOLUME_REG, volume);
}

void enable_channel(UINT8 channel, UINT8 tone_on, UINT8 noise_on) {
    UINT8 mixer;
    UINT8 tone;
    UINT8 noise;

    /* Validate values */
    if (channel > CHANNEL_C || tone_on > 1 || noise_on > 1) {
        return;
    }

    read_psg(MIXER_REG, &mixer);

    /* masks to isolate tone and noise bits */
    tone = 1 << channel;
    noise = 1 << (channel + 3);

    /* using masks, set/clear the channel's noise and tone bits*/
    if (tone_on == 0) {
        mixer |= tone;
    } else {
        mixer &= ~tone;
    }

    if (noise_on == 0) {
        mixer |= noise;
    } else {
        mixer &= ~noise;
    }

    write_psg(MIXER_REG, mixer);
}

void set_noise(UINT8 tuning) {
    /* Validate values */
    if ( tuning > MAX_NOISE) {
        return;
    }

    write_psg(NOISE_REG, tuning);
}

void set_envelope(UINT8 shape, UINT16 sustain) {
    UINT8 fine;
    UINT8 rough;
    
    /* Validate values */
    if ( shape > MAX_ENV) {
        return;
    }

    /* split sustain */
    fine = sustain & 0xFF;
    rough = sustain >> 8;

    write_psg(ENV_FINE_REG, fine);
    write_psg(ENV_RGH_REG, rough);

    write_psg(ENV_REG, shape); 

}

void stop_sound(void) {
    UINT8 mixer;
    int i;

    /* clear volume*/
    for (i = CHANNEL_A; i <= CHANNEL_C; i++) {
        set_volume(i, 0);
    }

    /* clear all noise and tone*/
    read_psg(MIXER_REG, &mixer);
    mixer |= DISABLE_NOISE;
    write_psg(MIXER_REG, mixer);


}