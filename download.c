#include <stdio.h>
#include <curl/curl.h>

#include "download.h"

// this is the index of the storedData array to write from
int writeDataI = 0;

static inline int existTest(const char *name) {
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
    for (int c = 0; c < size*nmemb; c++) {  
        storedData[writeDataI] = buf[c];
        writeDataI++;
    }
    // tell curl how many bytes we handled
    return size*nmemb;
}

int getHtml(const char* downloadUrl) {
    // our curl objects
    CURL* getHtml;
    CURLcode getHtmlResult;
    // initialise
    getHtml = curl_easy_init();
    // set url
    curl_easy_setopt(getHtml, CURLOPT_URL, downloadUrl);
    // send the html data to the call back function
    curl_easy_setopt(getHtml, CURLOPT_WRITEFUNCTION, &writeCallBack);
    // perform operation
    getHtmlResult = curl_easy_perform(getHtml);
   
    // if we fail
    if (getHtmlResult) {
        // reset the index
        writeDataI = 0;
        curl_easy_cleanup(getHtml);
        return 0;
    }
    writeDataI = 0;
    curl_easy_cleanup(getHtml);
    return 1;
}

int doDownload(char* url, char* file) {
    // our curl objects
    CURL* getImg;
    CURLcode getImgResult;
    FILE *img;

    // printf("%s\n", url);
    // only download if the file doesn't exist
    if (existTest(file) == 0) {
        // initialise
        getImg = curl_easy_init();
        // open the image file
        img = fopen(file, "w");
        // error if we cannot
        if (img == NULL) {
            printf("Unable To Create Image\n");
        }
        // set url
        curl_easy_setopt(getImg, CURLOPT_URL, url);
        curl_easy_setopt(getImg, CURLOPT_WRITEFUNCTION, NULL);
        // fail if we get an error
        curl_easy_setopt(getImg, CURLOPT_FAILONERROR, 1);
        // write data to the image file
        curl_easy_setopt(getImg, CURLOPT_WRITEDATA, img);
        // perform operation
        getImgResult = curl_easy_perform(getImg);

        // fail 
        if (getImgResult) {
            curl_easy_cleanup(getImg);
            fclose(img);
            printf("ERROR! on downloading pg.\n");
            return 0;
        }
        curl_easy_cleanup(getImg);
        fclose(img);
    }
    return 1; 
}