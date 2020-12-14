#ifndef PARSE_H
#define PARSE_H

struct nhentai_tags {
    int tag_sizes[10];
    char* title;
    char* gallery_id;
    char* parodies;
    char* characters;
    char* tags;
    char* artists;
    char* groups;
    char* languages;
    char* categories;
    char* pages;
};

typedef struct nhentai_tags ntags;

char* get_range(char data[], int start, int end);
ntags parse_tags(curl_memory data);

#endif // PARSE_H
