#ifndef NTAGS_H
#define NTAGS_H

#include "download.h"

typedef struct ntags {
    int sizes[7];
    
    char** parodies;
    char** characters;
    char** tags;
    char** artists;
    char** groups;
    char** languages;
    char** categories;

    char* data;
    int pages;
    char* title;
    char* gallery_id;
} ntags_T;

ntags_T* ntags_init(char* data);
void ntags_print(FILE* fp, ntags_T* ntags);
void ntags_info(ntags_T* ntags);
void ntags_tags(ntags_T* ntags);

#endif /* NTAGS_H */