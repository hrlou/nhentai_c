#pragma once

#include <stdlib.h>

typedef struct curl {
    char* data;
    size_t size;
} curl_T;

curl_T* curl_download_html(char* url);
int curl_download_file(char* url, char* file);
int curl_error_test(const char* url);