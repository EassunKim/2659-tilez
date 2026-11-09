#include <osbind.h>
#include "input.h"

BOOL has_input(void) {
    return(Cconis() != 0);
}

char get_input(void) {
    return((char)Cnecin());
}