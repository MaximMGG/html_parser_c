#include "../headers/htmlparser.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/m_string.h>
#include <util/m_list.h>

CURL *curl;
CURLcode res;
char content_was_null;

struct mem_t{
    char *data;
    size_t size;
};


static size_t mem_translate(void *data, size_t size, size_t size_m, void *user_data) {
    size_t real_size = size * size_m;
    struct mem_t *buf = (struct mem_t *)user_data;
    char *ptr;
    ptr = realloc(buf->data, real_size + buf->size + 1);

    if (!ptr) return 0;

    buf->data = ptr;
    memcpy(&(buf->data[buf->size]), data, real_size);
    buf->size += real_size;
    buf->data[buf->size] = 0;

    return real_size;
}

