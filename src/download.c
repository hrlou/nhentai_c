#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "download.h"
#include "tags.h"

static inline size_t exist_test(const char *name) {
    FILE *file;
    // if readible then return 1
    if ((file = fopen(name, "r")) != NULL) {
        fclose(file);
        return 1;
    }
    // if file is NULL it won't open, no memory leak
    return 0;
}

static size_t write_callback(char* buf, size_t size, size_t nmemb, void *pass) {
    size_t realsize = size*nmemb;
    // interpret the typless pass
    curl_memory *mem = (curl_memory *)pass;

    char* ptr = realloc(mem->data, mem->size + realsize + 1);
    if(ptr == NULL) {
        return 0;
    }
    mem->data = ptr;

    // copy buf (which contains the data in the curl buffer) into the newly re-allocated pointer
    memcpy(&(mem->data[mem->size]), buf, realsize);
    // iterate the size by the number of bytes handled
    mem->size += realsize;
    mem->data[mem->size] = 0;
    return realsize;
}

curl_memory get_html(char* input_url) {
    curl_memory html;
    html.size = 0;
    // this fixes realloc issues
    html.data = (char*) malloc(1);
    CURL* curl_get_html;
    // initialise
    curl_get_html = curl_easy_init();
    curl_easy_setopt(curl_get_html, CURLOPT_URL, input_url);
    // send the html data to the call back function
    curl_easy_setopt(curl_get_html, CURLOPT_WRITEFUNCTION, &write_callback);
    // give the callback function the file descriptor 
    curl_easy_setopt(curl_get_html, CURLOPT_WRITEDATA, (void *)&html);
    curl_easy_perform(curl_get_html);
    curl_easy_cleanup(curl_get_html);
    return html;
}

int download_gallery(char* gallery_id, int index, char* directory, char* extension) {
    // https://i.nhentai.net/galleries/ = 32
    // other stuff = 8
    char* url = (char *) malloc((40 + sizeof(gallery_id)));
    sprintf(url, "https://i.nhentai.net/galleries/%s/%d.%s", gallery_id, index, extension);
    char* file = (char *) malloc((sizeof(directory) + 30) * sizeof(char*));
    snprintf(file, ((sizeof(directory) + 30) * sizeof(char*)),  "%s/%03d.%s", directory, index, extension);

    // our curl objects
    CURL* get_img;
    CURLcode get_img_result;
    FILE *img_output;

    // printf("%s\n", url);
    // only download if the file doesn't exist
    if (exist_test(file) == 0) {
        get_img = curl_easy_init();
        img_output = fopen(file, "w");
        if (img_output == NULL) {
            return 0;
        }
        // set url
        curl_easy_setopt(get_img, CURLOPT_URL, url);
        free(url);
        curl_easy_setopt(get_img, CURLOPT_WRITEFUNCTION, NULL);
        // fail if we get an error
        curl_easy_setopt(get_img, CURLOPT_FAILONERROR, 1);
        // write data to the image file
        curl_easy_setopt(get_img, CURLOPT_WRITEDATA, img_output);
        get_img_result = curl_easy_perform(get_img);

        // page download fail
        if (get_img_result) {
            fclose(img_output);
            remove(file);
            curl_easy_cleanup(get_img);
            return 0;
        }
        curl_easy_cleanup(get_img);
        fclose(img_output);
    }
    return 1; 
}