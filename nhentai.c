#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#define HOME getenv("HOME")

typedef enum {false, true} bool;

char storedData[15000];
int writeDataI = 0;

size_t writeCallBack(char* buf, size_t size, size_t nmemb, void* up) {
    // callback must have this declaration
    // buf is a pointer to the data that curl has for us
    // size*nmemb is the size of the buffer
    // printf("\n\n");
    for (int c = 0; c < size*nmemb; c++) {
   		storedData[writeDataI] = buf[c];
   		writeDataI++;
    }

    return size*nmemb; // tell curl how many bytes we handled
}

int getGalleryId(char* downloadUrl) {
    CURL* getId; // our curl object
    CURLcode getIdResult;
    getId = curl_easy_init(); // easy int
    curl_easy_setopt(getId, CURLOPT_URL, downloadUrl); // make url the curl_url
    curl_easy_setopt(getId, CURLOPT_WRITEFUNCTION, &writeCallBack);
    getIdResult = curl_easy_perform(getId); // perform operation
   
    if (getIdResult) {
        curl_easy_cleanup(getId);
        return 0;
    }
    return 1;
}

#include "download.h"
#include "hegex.h"

// cc .. -lcurl

void main(int argc, const char **argv) {
	// 219076
	if (argc < 2) {
		exit(0);
	}

	// https://i.nhentai.net/galleries/1163557/1.jpg
	char* galleryId = malloc(10 * sizeof(char));
	char* galleryEx = malloc(8 * sizeof(char));
	char initialUrl[50];
	for (int currentPage = 1; currentPage < 30; currentPage++) {
		sprintf(initialUrl, "https://nhentai.net/g/%s/%d/", argv[1], currentPage);
		// yes we are redownloading the data every single time, some doujins have multiple image types
		getGalleryId(initialUrl);
		hegex_search(storedData, &galleryId, "https://i.nhentai.net/galleries/()/");
		char searchTemp[50];
		sprintf(searchTemp, "https://i.nhentai.net/galleries/%s/()\"", galleryId);
		hegex_search(storedData, &galleryEx, searchTemp);
		writeDataI = 0; 
		char* downloadUrl = malloc(60 * sizeof(char));
		sprintf(downloadUrl, "https://i.nhentai.net/galleries/%s/%s", galleryId, galleryEx);
		char* fileTemp = malloc(30 * sizeof(char));
		sprintf(fileTemp, "%s/%s", argv[1], galleryEx);
		mkdir(argv[1], 0700);
		doDownload(downloadUrl, fileTemp);
		free(downloadUrl);
		free(galleryId);
		free(galleryEx);
	}
}