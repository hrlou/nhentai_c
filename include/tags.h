#ifndef PARSE_H
#define PARSE_H

#include "download.h"

struct nhentai_comb {
    int pages;
    char* id;
    char* title;
    char* gallery_id;
    int sizes[7];
    char** parodies;
    char** characters;
    char** tags;
    char** artists;
    char** groups;
    char** languages;
    char** categories;
};


typedef struct nhentai_comb ntags;

// char* get_range(char data[], int start, int end);
// char* sanitise_tags(char* buf, size_t size);
// ntags parse_tags(curl_memory data);
ntags nhentai_tags(char* id);

#endif // PARSE_H
