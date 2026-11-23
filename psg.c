#include <osbind.h>
#include "psg.h"
/* ----- Function: read_psg -----
Purpose: Attempts to read a value from the given PSG register (0-15). This is a helper
         routine that will be helpful to test the other routines in this module.

Input: Register(UINT8) - The register that is being read from
       Value(UINT8&) - The value that was read from the register
                       Value is valid, if and only if the output is true

Output: Boolean indicating if a value was successfully read or not
*/
BOOL read_psg(UINT8 reg, UINT8 *value){
       long old_ssp;
       /*Pointer to PSG register selection/read address*/
       volatile UINT8 *PSG_reg_select = (volatile UINT8 *) 0xFF8800;

       /*Check invalid register or null pointer*/
       if (reg > 15 || value == 0){
              return FALSE;
       }

       old_ssp = Super(0);

       /*select register to read*/
       *PSG_reg_select = reg;

       /*read the selected registers val*/
       *value = *PSG_reg_select;

       Super(old_ssp);

       return TRUE;
}

/* ----- Function: write_psg -----
Purpose: Writes the given byte value (0-255) to the given PSG register (0-15). This is a helper
         routine to be used by the other functions in this module.

Input: Register(UINT8) - Register that is being written to
       Value(UINT8) - Value being written to reg

Output: None
*/
void write_psg(UINT8 reg, UINT8 val){
       volatile UINT8 *PSG_reg_select = (volatile UINT8 *) 0xFF8800;
       volatile UINT8 *PSG_reg_write = (volatile UINT8 *) 0xFF8802;
       long old_ssp;

       if (reg > 15){
              return;
       }

       old_ssp = Super(0);

       /*select register to read*/
       *PSG_reg_select = reg;

       *PSG_reg_write = val;

       Super(old_ssp);
}

/* ----- Function: set_tone -----
Purpose: Loads the tone registers (coarse and fine) for the given channel (0=A, 1=B, 2=C)
         with the given {12-bit} tuning.


Input: Channel(UINT8) - The channel (A, B, or C) that is being written to
       Tunning(UINT16) - 12-bit tone that will be written to the specified channel

Output: None
*/
void set_tone(UINT8 channel, UINT16 tuning){
       UINT8 fine, coarse;
       UINT8 reg;

       /*validate channel and 12 bit tuning*/
       if (channel > 2 || tuning > 0x0FFF){
              return;
       }

       
       reg = channel * 2;

       fine = tuning & 0xFF;
       coarse = (tuning >> 8) & 0x0F;

       write_psg(reg, fine);
       write_psg(reg + 1, coarse);
}

/* ----- Function: set_volume -----
Purpose: Writes the given byte value (0-255) to the given PSG register (0-15). This is a helper
         routine to be used by the other functions in this module.

Input: Channel(UINT8) - The channel (A, B, or C) that is being written to
       Volume(UINT8) - The volume value to be written to specified channel

Output: None
*/
void set_volume(UINT8 channel, UINT8 volume)   {

       if (channel > 2 || volume > 16){
              return;
       }

    /* Write volume to the channel's register */
    write_psg(8 + channel, volume);
}

/* ----- Function: set_noise -----
Purpose: Load the Noise register with the given tuning.

Input: Tunning(UINT8) - The 5-bit tuning value loaded to the noise register

Output: None
*/
void set_noise(UINT8 tuning){

       if (tuning > 31){
              return;
       }

       write_psg(6, tuning);
}

/* ----- Function: set_envelope -----
Purpose: Loads the PSG envelope control registers with the given envelope shape and 16-bit sustain.

Input: Shape(UINT8) - The shape loaded into the envelope control registers
       Sustain(UINT16) - The sustain value being written to the envelope control registers

Output: None
*/
void set_envelope(UINT8 shape, UINT16 sustain){

       if (shape > 15){
              return;
       }

       write_psg(11, sustain & 0xFF);
       write_psg(12, (sustain >> 8) & 0xFF);
       write_psg(13, shape);
}

/* ----- Function: stop_sound -----
Purpose: Silences all PSG sound production.

Input: None

Output: None
*/
void stop_sound(){
       set_volume(0, 0);
       set_volume(1, 0);
       set_volume(2, 0);
}

/* ----- Function: enable_channel -----
Purpose: Turns the given channel’s (0=A, 1=B, 2=C) tone/noise signals on/off (0=off, 1=on).

Input: Channel(UINT8) - The channel (A, B, or C) that is being written to
       Tone(UINT8) - Set the tone for the specified channel to be either on or off
       Noise(UINT8) - Set the noise for the specified channel to be either on or off

Output: None
*/
void enable_channel(UINT8 channel, UINT8 tone_on, UINT8 noise_on){
       UINT8 mixer;
       UINT8 tone_mask;
       UINT8 noise_mask;

       if (channel > 2 || tone_on > 1 || noise_on > 1){
              return;
       }

       if (!read_psg(7, &mixer)){
              return;
       }

       tone_mask = 1 << channel; 
       noise_mask = 1 << (channel + 3);

       if (tone_on){
              mixer &= ~tone_mask;
       } else {
              mixer |= tone_mask;
       }

       if (noise_on){
              mixer &= ~noise_mask;
       } else {
              mixer |= noise_mask;
       }

       write_psg(7, mixer);
}