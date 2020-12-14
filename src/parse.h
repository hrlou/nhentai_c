#ifndef PARSE_H
#define PARSE_H

struct nhentai_tags {
};

typedef struct nhentai_tags ntags;

char* get_range(char data[], int start, int end);
ntags parse_tags(curl_memory data);

#endif // PARSE_H
