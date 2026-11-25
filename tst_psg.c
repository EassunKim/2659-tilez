/*------------------------------------------------------------
 * File: tst_psg.c
 *
 * PURPOSE: Tests the functionality of the PSG library by verifying register values
 *          and audibly plays tone and envelope for further verification. 
 *          Includes a summary of passed and failed tests.    
 *
 * Team Members: Ujjwal, Matthew
 * Author: Ujjwal
 *------------------------------------------------------------*/


#include <stdio.h>
#include <osbind.h>
#include "psg.h"

static int passed = 0;
static int failed = 0;

/* ----- Function: Check_result -----
Purpose: Displays test results and updates counters

Input: Result(BOOL) - whether the test passes
       const char *name - Test name

Output: None
*/
void check_result(BOOL result, const char *name){
    if (result) {
        printf("PASS: %s\n", name);
        passed++;
    } else {
        printf("FAIL: %s\n", name);
        failed++;
    }
}

/* ----- Function: Check_register -----
Purpose: Compares conents of psg register with expected values

Input: Reg (UINT8) - Register being tested
       Expected(UINT8) - expected register value
       const char *name - Test name

Output: None
*/
void check_register(UINT8 reg, UINT8 expected, const char *name){
    UINT8 value;

    check_result(read_psg(reg, &value) && value == expected, name);
}
/* ----- Function: test_read_write -----
Purpose: Tests reading and writing PSG registers

Input: None

Output: None
*/
void test_read_write(){
    UINT8 value;

    write_psg(8, 11);
    check_register(8, 11, "Read/write PSG");

    check_result(!read_psg(16, &value), "Invalid register");
}
/* ----- Function: test_tone -----
Purpose: Tests setting fine and coarse tone

Input: None

Output: None
*/
void test_tone(){
    set_tone(0, 0x0ABC);

    check_register(0, 0xBC, "Fine tone");
    check_register(1, 0x0A, "Coarse tone");
}
/* ----- Function: test_volume -----
Purpose: Tests setting fixed volume and envelope volume

Input: None

Output: None
*/
void test_volume(){
    write_psg(8, 0);
    set_volume(0, 11);
    check_register(8, 11, "Fixed volume");

    write_psg(10, 0);
    set_volume(2, 16);
    check_register(10, 16, "Envelope volume mode");

    stop_sound();
}
/* ----- Function: test_noise -----
Purpose: Tests setting the noise frequency register

Input: None

Output: None
*/
void test_noise(){
    set_noise(15);
    check_register(6, 15, "Noise tuning");
}
/* ----- Function: test_enable_channel -----
Purpose: Tests enabling and disabling tone/noise while keeping
         settings of other channels

Input: None

Output: None
*/
void test_enable_channel(){
    UINT8 mixer;

    /* Enable tone A */
    enable_channel(0, 1, 0);
    check_result(read_psg(7, &mixer) &&
    (mixer & 0x3F) == 0x3E, "Enable tone A");

    /* Enable noise B while preserving tone A */
    enable_channel(1, 0, 1);
    check_result(read_psg(7, &mixer) &&
    (mixer & 0x3F) == 0x2E, "Enable noise B");

    /* Disable tone A while preserving noise B */
    enable_channel(0, 0, 0);
    check_result(read_psg(7, &mixer) &&
    (mixer & 0x3F) == 0x2F, "Disable tone A");

}

/* ----- Function: test_envelope -----
Purpose: Tests setting the envelope frequency and shape of envelope regisyers

Input: None

Output: None
*/void test_envelope(){
    set_envelope(9, 0x1234);

    check_register(11, 0x34, "Fine envelope");
    check_register(12, 0x12, "Coarse envelope");
    check_register(13, 9, "Envelope shape");
}
/* ----- Function: test_stop_sound -----
Purpose: Tests if all three channels silence

Input: None

Output: None
*/
void test_stop_sound(){
    set_volume(0, 11);
    set_volume(1, 12);
    set_volume(2, 16);

    stop_sound();

    check_register(8, 0, "Silence channel A");
    check_register(9, 0, "Silence channel B");
    check_register(10, 0, "Silence channel C");
}

/* ----- Function: test_audio_tone -----
Purpose: Audible tests for enabling and disabling tone

Input: None

Output: None
*/
void test_audio_tone(){
    set_tone(0, 250);
    set_volume(0, 15);
    enable_channel(0, 1, 0);

    printf("Tone ON. Press a key\n");
    Cnecin();

    enable_channel(0, 0, 0);

    printf("Tone OFF. Press a key\n");
    Cnecin();

    stop_sound();
}

/* ----- Function: test_audio_envelope -----
Purpose: Audible tests for envelope sound

Input: None

Output: None
*/
void test_audio_envelope(){
    printf("Playing envelope effect on channel C.\n");

    set_tone(2, 250);
    enable_channel(2, 1, 0);
    set_volume(2, 16);
    set_envelope(9, 5000);

    Cnecin();
    stop_sound();
}

/* ----- Function: print_results -----
Purpose: Prints total number of tests passed/failed

Input: None

Output: None
*/
void print_results(){
    printf("\nPSG tests complete.\n");
    printf("Tests passed: %d\n", passed);
    printf("Tests failed: %d\n", failed);

}
/* ----- Function: main -----
Purpose: Initializes the psg, then runs tests, and finally displays results

Input: None

Output: None
*/

int main(){
    UINT8 original_mixer;

    /* Silence PSG and disable tone/noise */
    if (!read_psg(7, &original_mixer))
        return 1;

    stop_sound();
    write_psg(7, original_mixer | 0x3F);

    test_read_write();
    test_tone();
    test_volume();
    test_noise();
    test_enable_channel();
    test_envelope();
    test_stop_sound();

    test_audio_tone();
    test_audio_envelope();

    print_results();

    return 0;
}