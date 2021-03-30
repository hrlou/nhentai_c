#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "search.h"
#include "nhentai.h"

static char* search_url_gen(search_T* search) {
    char* url = calloc((60 + strlen(search->term)), sizeof(char));
    snprintf(url, (60 + strlen(search->term)), "https://nhentai.net/search/?q=%s&sort=popular&page=%d", search->term, search->page);
    return url;
}

search_T* search_init(const char* term) {
    search_T* search = calloc(1, sizeof(struct search));
    search->term = calloc((1 + strlen(term)), sizeof(char));
    search->results = calloc(1, sizeof(struct nhentai*));
    search->count = 0;
    strcpy(search->term, term);
    for (int i = 0; search->term[i]; i++) {
        if (search->term[i] == ' ') {
            search->term[i] = '+';
        }
    }
    search->page = 1;
    return search;
}

void search_add_result(search_T* search, char* id) {
    search->results = realloc(search->results, (1 + search->count) * sizeof(struct nhentai*));
    search->results[search->count] = nhentai_init(id);
    search->count++;
    return;
}

int search_parse(search_T* search) {
    char* s = search->mem->data;
    if (strstr(s, "No results found") != NULL) {
        return 0;
    }
    for (; *s; s++) {
        if (strncmp(s, "a href=\"/g/", 11) == 0) {
            s+=11;
            char* id = calloc(10, sizeof(char));
            for (int i = 0; *s != '/'; s++, i++) {
                id[i] = *s;
            }
            search_add_result(search, id);
        }
    }
    return 1;
}

int search_data(search_T* search) {
    search->mem = curl_download_html(search_url_gen(search));
    if (search->mem->size == 0) {
        fputs("failure downloading search\n", stderr);
        return 0;
    }
    return search_parse(search);
}

void search_execute(search_T* search) {
    for (; search_data(search); search->page++);
}