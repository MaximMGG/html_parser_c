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
    char type_name[32];
    char type_content[128];
} tag_type;

typedef struct {
    char tag[32];
    tag_type t_type[8];
    unsigned int type_count;
} TAG;

static void Tag_free(void *temp) {
    free(temp);
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
    return 0;
}


static PARSER_CODE Parser_pars_one_tag(char *open_tag, char *close_tag, html_content *page) {

    return 0;
}

//body > div[class(main)]
//link[rel type href]
static List *Parser_list_expression(const char *expression) {
    List *tags_expression = list_create(0, l_struct);
    tags_expression->type_size = sizeof(TAG);
    int exp_len = strlen(expression);

    char buf[128];
    boolean in_type = false;
    boolean type_has_content = false;
    int bc = 0;
    TAG *t;

    for(int i = 0; i < exp_len; i++) {
        t = malloc(sizeof(TAG));
        if (expression[i] == '>') {
            list_add(tags_expression, t);
            memset(buf, 0, 128);
            bc = 0;
        } else if (expression[i] == ' ' && in_type) {
            if (type_has_content) {
                memset(buf, 0, 128);
                bc = 0;
            } else {
                strcpy(t->t_type[t->type_count++].type_name, buf);
                memset(buf, 0, 128);
                bc = 0;
            }
        } else if (expression[i] == ' ') {
            strcpy(t->tag, buf);
            memset(buf, 0, 128);
            bc = 0;
        } else if (expression[i] == '[') {
            in_type = true;
        } else if (expression[i] == ']') {
            in_type = false;
            if (type_has_content) {
                memset(buf, 0, 128);
                bc = 0;
                type_has_content = false;
            } else {
                strcpy(t->t_type[t->type_count++].type_name, buf);
                memset(buf, 0, 128);
                bc = 0;
            }
        } else if (expression[i] == '(') {
            type_has_content = true;
            strcpy(t->t_type[t->type_count++].type_name, buf);
            memset(buf, 0, 128);
            bc = 0;
        } else if (expression[i] == ')') {
            strcpy(t->t_type[t->type_count++].type_content, buf);
            memset(buf, 0, 128);
            bc = 0;
        } else {
            buf[bc++] = expression[i];
        }
    }

    if (in_type) {
        list_add(tags_expression, t);
    } else {
        strcpy(t->tag, buf);
        list_add(tags_expression, t);
    }

    return tags_expression;
}



PARSER_CODE Parser_pars(const char *expression, html_content *page) {
    List *tags_expression = Parser_list_expression(expression);
    char open_tag[264] = {0};
    char close_tag[32] = {0};

    for(int i = 0; i < tags_expression->len; i++) {
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
