#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <regex.h>

#define HOME getenv("HOME")

int mkdir(const char *pathname, mode_t mode);

char storedData[15000];
int writeDataI = 0;

static inline int existTest(const char *name) {
	FILE *file;
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

#include "download.h"

#define GALLERYID pmatch[1].rm_eo - pmatch[1].rm_so, &storedData[pmatch[1].rm_so]
#define GALLERYEX pmatch[2].rm_eo - pmatch[2].rm_so, &storedData[pmatch[2].rm_so]

void doProcess(const char *doujin) {
	// this is temporary
	regmatch_t pmatch[2];
	regex_t preg;
	char downloadUrl[60];
	char fileTemp[30];
	char currentUrl[35];
	sprintf(currentUrl, "https://nhentai.net/g/%s/1/", doujin);
	puts(currentUrl);
	getHtml(currentUrl);
	regcomp(&preg, "https://i.nhentai.net/galleries/\\(.*\\)/[0-9].\\([a-z].*\\)\" w", 0);
	regexec(&preg, storedData, 3, pmatch, 0);
	for (int i = 1; i < 28; i++) {
		sprintf(downloadUrl, "https://i.nhentai.net/galleries/%.*s/%d.%.*s", GALLERYID, i, GALLERYEX);
		puts(downloadUrl);
	}
	regfree(&preg);
}

void main(int argc, const char **argv) {
	// 219076
	// 165598, page 2 is a png

	if (argc < 2) {
		printf("NO ID SPECIFIED EXITING\n");
		exit(0);
	}
	doProcess(argv[1]);
	doProcess(argv[2]);
	// mkdir(argv[1], 0700);
	// doDownload(downloadUrl, fileTemp);
}