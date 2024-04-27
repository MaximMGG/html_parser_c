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

typedef struct {
    char tag[32];
    char close_tag[32];
    char *tag_type;
    char *tag_value;
    char tag_velosity;
} TAG;


static void Tag_free(void *temp) {
    TAG *t = (TAG *) temp;
    free(t->tag_type);
    free(t->tag_value);
    free(t);
}

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

static int Parser_find_first(html_content *html, STR tag, unsigned int offset) {
    int pos = 0;
    char same = 0;
    for(int i = offset, j = 0; i < html->size; i++, j++) {
        if (tag[j] == html->html[i]) {
            same = 1;
            pos = i;
        } else if(same == 1) {
            if (STRLEN(tag) == j - 1) {
                return pos;
            }
        }
    }

    return 0;
}


static PARSER_CODE Parser_pars_one_tag(char *open_tag, char *close_tag, html_content *page) {
    int open_pos = Parser_find_first(page, newstr(open_tag), 0);
    int close_pos = Parser_find_first(page, newstr(close_tag), open_pos);
    char *buf = malloc(sizeof(char) * close_pos - open_pos + 2);

    if (close_pos - open_pos < 1) {
        return PARSER_PARS_ERROR;
    }
    memcpy(buf, &page->html[open_pos], close_pos - open_pos);
    if (page->parser_content == NULL) {
        page->parser_content = malloc(sizeof(char) * close_pos - open_pos + 1);
    } else {
        page->parser_content = realloc(page->parser_content, sizeof(char) * close_pos - open_pos + 1);
    } 

    strcpy(page->parser_content, buf);

    free(buf);
    return PARSER_OK;
}

static List *Parser_list_expression(const char *expression) {
    List *tags_expression = list_create(0, l_struct);
    tags_expression->type_size = sizeof(TAG);
    int exp_len = strlen(expression);

    char buf[128];
    int bc = 0;
    int tags = 0;

    for(int i = 0; i < exp_len; i++) {
        TAG *t = (TAG *) malloc(sizeof(TAG));
        t->tag_velosity = 0;
        memset(t->tag, 0, 32);
        memset(t->close_tag, 0, 32);
        if (expression[i] == '>') {
            list_add(tags_expression, t);
        } else if (expression[i] == ' ') {
            switch (tags) {
                case 0 :{
                    strcpy(t->tag, buf);
                    memset(buf, 0, 128);
                    bc = 0;
                } break;
                case 1: {
                    t->tag_type = malloc(sizeof(char) * strlen(buf) + 1);
                    strcpy(t->tag_type, buf);
                    memset(buf, 0, 128);
                    bc = 0;
                } break;
                case 2:{
                    t->tag_value = malloc(sizeof(char) * strlen(buf) + 1);
                    strcpy(t->tag_value, buf);
                    memset(buf, 0, 128);
                    bc = 0;
                } break;
            }
            tags++;
            t->tag_velosity++;
        }
        else {
            buf[bc++] = expression[i];
        }
    }
    return tags_expression;
}



PARSER_CODE Parser_pars(const char *expression, html_content *page) {
    List *tags_expression = Parser_list_expression(expression);
    char open_tag[264] = {0};
    char close_tag[32] = {0};

    for(int i = 0; i < tags_expression->len; i++) {
        TAG *t = (TAG *) list_get(tags_expression, i);
        switch(t->tag_velosity) {
            case 0: {
                fprintf(stderr, "Error, tag_velosity is 0\n");
                return PARSER_PARS_ERROR;
            } break;
            case 1: {
                snprintf(open_tag, 264, "<%s>", t->tag);
                snprintf(close_tag, 32, "</%s>", t->tag);
            } break;
            case 2: {
                snprintf(open_tag, 264, "<%s %s=\"\">", t->tag, t->tag_type);
                snprintf(close_tag, 32, "</%s>", t->tag);
            }
        }

    }





    list_free_all_struct(tags_expression, Tag_free);
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
