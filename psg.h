#ifndef PSG
#define PSG

/* CHANGES MADE TO THIS FILE:
Added in my own types.h for BOOL and UINT
rewrote header for volume function to better describe the function
Added in the below defines to make psg.c more readable
*/
#include "types.h"

/* read/write registers */
#define PSG_SELECT 0xFF8800
#define PSG_WRITE 0xFF8802

/* Channels*/
#define CHANNEL_A 0
#define CHANNEL_B 1
#define CHANNEL_C 2

/*Max values for testing*/
#define MAX_TONE 0x0FFF
#define MAX_VOLUME 15
#define REG_COUNT 16
#define MAX_NOISE 0x1F /* 5 used bits all set , via the documentation on d2l*/

/*Register constants*/
#define VOLUME_REG 8
#define MIXER_REG 7
#define NOISE_REG 6
#define ENV_REG 0xD



/* ----- Function: read_psg -----
Purpose: Attempts to read a value from the given PSG register (0-15). This is a helper
         routine that will be helpful to test the other routines in this module.

Input: Register(UINT8) - The register that is being read from
       Value(UINT8&) - The value that was read from the register
                       Value is valid, if and only if the output is true

Output: Boolean indicating if a value was successfully read or not
*/
BOOL read_psg(UINT8 reg, UINT8 *value);

/* ----- Function: write_psg -----
Purpose: Writes the given byte value (0-255) to the given PSG register (0-15). This is a helper
         routine to be used by the other functions in this module.

Input: Register(UINT8) - Register that is being written to
       Value(UINT8) - Value being written to reg

Output: None
*/
void write_psg(UINT8 reg, UINT8 val);

/* ----- Function: set_tone -----
Purpose: Loads the tone registers (coarse and fine) for the given channel (0=A, 1=B, 2=C)
         with the given {12-bit} tuning.


Input: Channel(UINT8) - The channel (A, B, or C) that is being written to
       Tunning(UINT16) - 12-bit tone that will be written to the specified channel

Output: None
*/
void set_tone(UINT8 channel, UINT16 tuning);

/* ----- Function: set_volume -----
Purpose: used to adjust the volume of the given channel to the given tuning value

Input: Channel(UINT8) - The channel (A, B, or C) that is being written to
       Volume(UINT8) - The volume value to be written to specified channel

Output: None
*/
void set_volume(UINT8 channel, UINT8 volume);

/* ----- Function: enable_channel -----
Purpose: Turns the given channel’s (0=A, 1=B, 2=C) tone/noise signals on/off (0=off, 1=on).

Input: Channel(UINT8) - The channel (A, B, or C) that is being written to
       Tone(UINT8) - Set the tone for the specified channel to be either on or off
       Noise(UINT8) - Set the noise for the specified channel to be either on or off

Output: None
*/
void enable_channel(UINT8 channel, UINT8 tone_on, UINT8 noise_on);

/* ----- Function: set_noise -----
Purpose: Load the Noise register with the given tuning.

Input: Tunning(UINT8) - The 5-bit tuning value loaded to the noise register

Output: None
*/
void set_noise(UINT8 tuning);

/* ----- Function: set_envelope -----
Purpose: Loads the PSG envelope control registers with the given envelope shape and 16-bit sustain.

Input: Shape(UINT8) - The shape loaded into the envelope control registers
       Sustain(UINT16) - The sustain value being written to the envelope control registers

Output: None
*/
void set_envelope(UINT8 shape, UINT16 sustain);

/* ----- Function: stop_sound -----
Purpose: Silences all PSG sound production.

Input: None

Output: None
*/
void stop_sound();
#endif