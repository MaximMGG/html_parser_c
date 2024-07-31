#include "../headers/hp_error.h"
#include <stdio.h>
#include <stdlib.h>

char *hp_code_error[] = {
    "HP_STATUS_OK", 
};

char *hp_get_error(HP_CODE code) {
    return hp_code_error[code];
}

void hp_print_error(const char *format, ...) {
    va_list li;
    va_start(li, format);
    vfprintf(stderr, format, li);
    va_end(li);
    fflush(stderr);
}
void hp_print_faltal(const char *format, ...) {
    va_list li;
    va_start(li, format);
    vfprintf(stderr, format, li);
    va_end(li);
    fflush(stderr);
    exit(EXIT_FAILURE);

}
