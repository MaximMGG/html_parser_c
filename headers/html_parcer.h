#ifndef HTML_PARSER_H
#define HTML_PARSER_H
#include <cstdext/core.h>
#include <cstdext/estring.h>
#include <cstdext/arraylist.h>

#define NONULL(...) __attribute__((__nonnull__(__VA_ARGS__)))

typedef struct {
    char *teg;
    char *teg_class;
    char *teg_class_param;

    u32 skip;
} hp_teg;

typedef struct {
    char *request;
    Arraylist *param;

    char *content;
} hp_request;

typedef struct {
    str html_content;
    u32 content_len;

    str request;
} hp_html;

char *hp_get_html_content(const char *request, const char *url) NONULL(1, 2);

#endif //HTML_PARSER_H
