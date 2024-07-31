#ifndef HTML_PARSER_H
#define HTML_PARSER_H
#include <cstdext/core.h>
#include <cstdext/estring.h>

typedef struct {
    str html_content;
    u32 content_len;

    str request;
} htmlp;

#endif //HTML_PARSER_H
