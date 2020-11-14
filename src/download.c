#include <curl/curl.h>
#include <stdlib.h>

#include <stdio.h>

#include "main.h"
#include "download.h"
#include "../config.def.h"
#include "global.h"

// this is the index of the storedData array to write from
int writeDataI = 0;
int tagsDataI = 0;

static inline size_t existTest(const char *name) {
    FILE *file;
    // if readible then return 1
    if ((file = fopen(name, "r")) != NULL) {
        fclose(file);
        return 1;
    }
    // if file is NULL it won't open, no memory leak
    return 0;
}

static size_t writeCallBack(char* buf, size_t size, size_t nmemb, void* up) {
    // buf is a pointer to the data that curl has for us
    // size*nmemb is the size of the buffer
    static size_t writing = 1;
    static size_t newline = 0;
    if (writeDataI == 0) {
        newline = 0;
    }
    for (int c = 0; c < size*nmemb; c++) {
        // this is horrible, but it saves RAM and makes parsing easier
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
                storedData[writeDataI] = '-';
                writeDataI++;
            } else if (buf[c] != '\t') {
                storedData[writeDataI] = buf[c];
                writeDataI++;
            }    
        }
    }
    // tell curl how many bytes we handled
    return size*nmemb;
}

int getHtml(char* downloadUrl) {
    // our curl objects
    CURL* getHtml;
    CURLcode getHtmlResult;
    // initialise
    getHtml = curl_easy_init();
    curl_easy_setopt(getHtml, CURLOPT_URL, downloadUrl);
    // send the html data to the call back function
    curl_easy_setopt(getHtml, CURLOPT_WRITEFUNCTION, &writeCallBack);
    getHtmlResult = curl_easy_perform(getHtml);
    // if we fail
    if (getHtmlResult) {
        // need to reset the index
        writeDataI = 0;
        curl_easy_cleanup(getHtml);
        return 0;
    }
    writeDataI = 0;
    curl_easy_cleanup(getHtml);
    return 1;
}

int doDownload(char* gId, int gIndex, char* dDir, char* ext) {
    // https://i.nhentai.net/galleries/ = 32
    // other stuff = 8

    char *url = (char *) malloc((40 + sizeof(gId)));
    sprintf(url, "https://i.nhentai.net/galleries/%s/%d.%s", gId, gIndex, ext);
    char *file = (char *) malloc((sizeof(dDir) + 30) * sizeof(char*));
    // char *file = (char *) malloc(LIMIT + 15);
    snprintf(file, ((sizeof(dDir) + 30) * sizeof(char*)),  "%s/%03d.%s", dDir, gIndex, ext);

    // our curl objects
    CURL* getImg;
    CURLcode getImgResult;
    FILE *img;

    // printf("%s\n", url);
    // only download if the file doesn't exist
    if (existTest(file) == 0) {
        getImg = curl_easy_init();
        img = fopen(file, "w");
        if (img == NULL) {
            return 0;
        }
        // set url
        curl_easy_setopt(getImg, CURLOPT_URL, url);
        free(url);
        curl_easy_setopt(getImg, CURLOPT_WRITEFUNCTION, NULL);
        // fail if we get an error
        curl_easy_setopt(getImg, CURLOPT_FAILONERROR, 1);
        // write data to the image file
        curl_easy_setopt(getImg, CURLOPT_WRITEDATA, img);
        getImgResult = curl_easy_perform(getImg);

        // page download fail
        if (getImgResult) {
            fclose(img);
            remove(file);
            curl_easy_cleanup(getImg);
            return 0;
        }
        curl_easy_cleanup(getImg);
        fclose(img);
    }
    return 1; 
}
