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