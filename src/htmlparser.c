#include "../headers/htmlparser.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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



PARSER_CODE Parser_init(const char *url, html_content *content) {
    curl = curl_easy_init();
    if (content == NULL) {
        content_was_null = 1;
        content = (html_content *) malloc(sizeof(html_content));
    }
    
    struct mem_t m = {0};

    if (url == NULL) {
        if (content_was_null) {
            free(content);
        }
        curl_easy_cleanup(curl);
        return PARSER_BAD_URL;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m);

    if (m.size <= 1) {
        return PARSER_WRITEFUNC_ERROR;
    }
    content->html = malloc(sizeof(char) * m.size);
    content->size = m.size;
    content->parser_content = NULL;

    return PARSER_OK;
}



static PARSER_CODE Parser_pars_one_tag(char *tag, html_content *page) {
    char *buf = malloc(sizeof(char) * page->size);





    free(buf);
    return PARSER_OK;
}


PARSER_CODE Parser_pars(const char *expression, html_content *page) {

    return PARSER_OK;
}

void Paresr_cleanup(html_content *page) {
    if (page != NULL) {
        if (page->html == NULL) {
            if (page->parser_content == NULL) {
                free(page);
                return;
            } else {
                free(page->parser_content);
                free(page);
                return;
            }
        } else {
            free(page->html);
            if (page->parser_content == NULL) {
                free(page);
                return;
            } else {
                free(page->parser_content);
                free(page);
                return;
            }
        }
    }
    curl_easy_cleanup(curl);
}
