#ifndef DOWNLOAD_H
#define DOWNLOAD_H

struct curl_memory_store {
    char *data;
    size_t size;
};

typedef struct curl_memory_store curl_memory;

static inline size_t exist_test(const char *name);
static size_t write_callback(char* buf, size_t size, size_t nmemb, void *pass);
curl_memory get_html(char* input_url);
int download_gallery(char* gallery_id, int gallery_index, char* download_directory, char* download_extension);

#endif // DOWNLOAD_H