#ifndef SEARCH_H
#define SEARCH_H

#include <stdlib.h>

#include "nhentai.h"
#include "curl.h"

typedef struct search {
    char* term;
    unsigned int page;
    size_t count;
    nhentai_T** results;
    curl_T* mem;
} search_T;

search_T* search_init(const char* term);
void search_add_result(search_T* search, char* id);
int search_parse(search_T* search);
int search_data(search_T* search);
void search_execute(search_T* search);
search_T* nhentai_search(const char* term);

#endif /* SEARCH_H */