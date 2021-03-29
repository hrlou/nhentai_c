#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <stdlib.h>

typedef struct curl {
    char* data;
    size_t size;
} curl_T;

curl_T* download_html(char* url);
int download_file(char* url, char* file);

#endif /* DOWNLOAD_H */