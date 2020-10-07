#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#define HOME getenv("HOME")

typedef enum {false, true} bool;

char storedData[10000];
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

#include "gallery.h"
#include "download.h"

// cc .. -lcurl

void main(int argc, const char **argv) {
	// https://i.nhentai.net/galleries/1163557/1.jpg
	char* galleryId = malloc(10 * sizeof(char));
	char* galleryEx = malloc(8 * sizeof(char));
	char initialUrl[100];
	for (int currentPage = 1; currentPage < 30; currentPage++) {
		sprintf(initialUrl, "https://nhentai.net/g/%s/%d/", argv[1], currentPage);
		getGalleryId(initialUrl);
		searchGalleryInfo(&galleryId, &galleryEx);
		writeDataI = 0;
		char* downloadUrl = malloc(60 * sizeof(char));
		sprintf(downloadUrl, "https://i.nhentai.net/galleries/%s/%s", galleryId, galleryEx);
		// printf("%s", downloadUrl);
		doDownload(downloadUrl, galleryEx);
		free(downloadUrl);
		free(galleryId);
		free(galleryEx);
	}
}