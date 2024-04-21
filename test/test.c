#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>


struct mem {
    char *data;
    unsigned int size;
};

static unsigned int write_byte_func(void *data, unsigned int size, unsigned int mbsize, void *client) {
    unsigned int realsize = size * mbsize;
    struct mem *m = (struct mem *) client;

    m->data = realloc(m->data, realsize + m->size + 1);
    if (m->data == NULL) {
        return 0;
    }

    memcpy(&(m->data[m->size]), data, realsize);

    m->size += realsize;
    m->data[m->size] = 0;

    return realsize;
}



int main() {

    CURL *curl;
    CURLcode res;
    struct mem chunk = {0};

    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "example.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_byte_func);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

    curl_easy_perform(curl);


    printf("------\n"); 
    printf("%s\n", chunk.data);
    printf("------\n"); 

    free(chunk.data);
    curl_easy_cleanup(curl);

    return 0;
}
