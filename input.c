#include <osbind.h>
#include "input.h"

BOOL has_input(void) {
    return(Cconis() != 0);
}

char get_input(void) {
    if (!has_input()) {
        return 0;
    } else {
    return((char)Cnecin());
    }
}