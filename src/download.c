#include <curl/curl.h>
#include <stdlib.h>

#include "main.h"
#include "download.h"
#include "../config.def.h"

// this is the index of the stored_data array to write from
int write_data_iterate = 0;

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

static size_t write_callback_doujin(char* buf, size_t size, size_t nmemb, void* up) {
    /*  buf is a pointer to the data that curl has for us
        size*nmemb is the size of the buffer    */
    static size_t writing = 1;
    static size_t newline = 0;
    if (write_data_iterate == 0) {
        newline = 0;
    }
    for (int c = 0; c < size*nmemb; c++) {
        // this is horrible, but it saves memory and makes parsing easier
        if (buf[c] == '\n') {
            newline++;
        }
        if (buf[c] == '<' || buf[c] == '\\') {
            writing = 0;
        }
        if (buf[c] == '>') {
            writing = 1;
        }
        if (newline < 5) {
            writing = 1;
        }
        if (writing == 1) {
            if (buf[c] == ' ') {
                stored_data[write_data_iterate] = '-';
                write_data_iterate++;
            } else if (buf[c] != '\t') {
                stored_data[write_data_iterate] = buf[c];
                write_data_iterate++;
            } else if (buf[c] == '&' && buf[c + 1] == '#' && buf[c + 2] == '3') {
                stored_data[write_data_iterate] = '\'';
                c+=4;
                write_data_iterate++;
            }
        }
    }
    // tell curl how many bytes we handled
    return size*nmemb;
}

static size_t write_callback_search(char* buf, size_t size, size_t nmemb, void* up) {
    for (int c = 0; c < size*nmemb; c++) {
        if (write_data_iterate >= 30000) {
            return 0;
        }
        if (buf[c] == '&' && buf[c + 1] == '#' && buf[c + 2] == '3') {
            stored_data[write_data_iterate] = '\'';
            c+=4;
            write_data_iterate++;
        } else {
            stored_data[write_data_iterate] = buf[c];
            write_data_iterate++;
        }
    }
    // tell curl how many bytes we handled
    return size*nmemb;
}


int get_html(char* download_url, int option) {
    // our curl objects
    CURL* get_html;
    CURLcode get_html_result;
    // initialise
    get_html = curl_easy_init();
    curl_easy_setopt(get_html, CURLOPT_URL, download_url);
    // send the html data to the call back function
    if (option == 0) {
        curl_easy_setopt(get_html, CURLOPT_WRITEFUNCTION, &write_callback_doujin);
    } else if (option == 1) {
        curl_easy_setopt(get_html, CURLOPT_WRITEFUNCTION, &write_callback_search);
    }
    get_html_result = curl_easy_perform(get_html);
    // if we fail
    if (get_html_result) {
        // need to reset the index
        write_data_iterate = 0;
        curl_easy_cleanup(get_html);
        return 0;
    }
    write_data_iterate = 0;
    curl_easy_cleanup(get_html);
    return 1;
}

int download_gallery(char* gallery_id, int gallery_index, char* download_directory, char* download_extension) {
    // https://i.nhentai.net/galleries/ = 32
    // other stuff = 8

    char *url = (char *) malloc((40 + sizeof(gallery_id)));
    sprintf(url, "https://i.nhentai.net/galleries/%s/%d.%s", gallery_id, gallery_index, download_extension);
    char *file = (char *) malloc((sizeof(download_directory) + 30) * sizeof(char*));
    // char *file = (char *) malloc(LIMIT + 15);
    snprintf(file, ((sizeof(download_directory) + 30) * sizeof(char*)),  "%s/%03d.%s", download_directory, gallery_index, download_extension);

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
