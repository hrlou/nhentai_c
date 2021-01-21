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

void nhentai_essential(char* s, ntags* tags);
void nhentai_ident(char* o, ntags* tags);
ntags nhentai_tags(char* id);

#endif // PARSE_H
