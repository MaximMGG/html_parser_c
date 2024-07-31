#ifndef HP_ERROR_H
#define HP_ERROR_H

#include <stdarg.h>

typedef int HP_CODE;

#define NORETURN __attribute__((__noreturn__))

char *hp_get_error(HP_CODE code);
void hp_print_error(const char *format, ...);
void hp_print_faltal(const char *format, ...) NORETURN;


#endif //HP_ERROR_H
