#include "../headers/hpnet.h"
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


struct mem {
    char *m;
    size_t len;
};

static size_t write_func(void *data, size_t size, size_t memb, void *client) {
    size_t realsize = size * memb;
    struct mem *m = (struct mem *) client;
    char *p = realloc(m->m, m->len + realsize + 1);

    m->m = p;
    memcpy(&(m->m[m->len]), data, realsize);
    m->len += realsize;
    m->m[m->len] = 0;

    return realsize;
}


char *hpnet_get_html(const char *url) {
    struct mem m = {0};
    m.m = malloc(sizeof(char));
    CURLcode res;
    CURL *curl;
    curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&m);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);

    res = curl_easy_perform(curl);
    
    curl_easy_cleanup(curl);

    return m.m;
}
