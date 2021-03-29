#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "curl.h"

static inline size_t exist_test(const char *name) {
    FILE *file;
    /* if readible then return 1 */
    if ((file = fopen(name, "r")) != NULL) {
        fclose(file);
        return 1;
    }
    /* if file is NULL it won't open, no memory leak */
    return 0;
}

static size_t write_callback(char* buf, size_t size, size_t nmemb, void *pass) {
    /* interpret the typless pass */
    curl_T* mem = (curl_T*)pass;
    char* ptr = realloc(mem->data, mem->size + size*nmemb + 1);
    if (ptr == NULL) {
        return 0;
    }
    mem->data = ptr;
    /* copy buf (which contains the data in the curl buffer) into the newly re-allocated pointer */
    memcpy(&(mem->data[mem->size]), buf, size*nmemb);
    /* increase the size by the number of bytes handled */
    mem->size += size*nmemb;
    mem->data[mem->size] = 0;
    return size*nmemb;
}

curl_T* curl_download_html(char* url) {
    curl_T* mem = calloc(1, sizeof(struct curl));
    mem->size = 0;
    mem->data = calloc(1, sizeof(char));
    CURL* curl_get_html = curl_easy_init();
    curl_easy_setopt(curl_get_html, CURLOPT_URL, url);
    /* send the data to the call back function */
    curl_easy_setopt(curl_get_html, CURLOPT_WRITEFUNCTION, &write_callback);
    /* give the callback function mem struct */
    curl_easy_setopt(curl_get_html, CURLOPT_WRITEDATA, (void *)mem);
    /* fail if we get a 404 or any other error, size will be 0 to identify */
    curl_easy_setopt(curl_get_html, CURLOPT_FAILONERROR, 1);
    curl_easy_perform(curl_get_html);
    curl_easy_cleanup(curl_get_html);
    return mem;
}

int curl_download_file(char* url, char* output) {
    /* our curl objects */
    CURL* get_file;
    CURLcode get_file_result;
    FILE* file_output;

    /* only download if the file doesn't exist */
    if (!exist_test(output)) {
        get_file = curl_easy_init();
        file_output = fopen(output, "w");
        if (file_output == NULL) {
            return 0;
        }
        /* set url */
        curl_easy_setopt(get_file, CURLOPT_URL, url);
        curl_easy_setopt(get_file, CURLOPT_WRITEFUNCTION, NULL);
        /* fail if we get an error */
        curl_easy_setopt(get_file, CURLOPT_FAILONERROR, 1);
        /* write data to the image file */
        curl_easy_setopt(get_file, CURLOPT_WRITEDATA, file_output);
        get_file_result = curl_easy_perform(get_file);

        /* file download fail */
        if (get_file_result) {
            fclose(file_output);
            remove(output);
            curl_easy_cleanup(get_file);
            return 0;
        }
        curl_easy_cleanup(get_file);
        fclose(file_output);
    }
    return 1; 
}

int curl_error_test(const char* url) {
    CURL* exist = curl_easy_init();
    curl_easy_setopt(exist, CURLOPT_URL, url);
    curl_easy_setopt(exist, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(exist, CURLOPT_FAILONERROR, 1);
    CURLcode exist_result = curl_easy_perform(exist);
    if (exist_result) {
        curl_easy_cleanup(exist);
        return 0;
    }
    return 1;
}