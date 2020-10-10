#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#define HOME getenv("HOME")

#include "main.h"
#include "download.h"

#define GALLERYID pmatch[1].rm_eo - pmatch[1].rm_so, &storedData[pmatch[1].rm_so]
#define GALLERYEX pmatch[2].rm_eo - pmatch[2].rm_so, &storedData[pmatch[2].rm_so]

// function is temporary; make debbuging easier
void doProcess(const char *doujin) {
	// regex objects
	regmatch_t pmatch[2];
	// ge; Gallery, Extension
	regex_t gereg;
	// temporary name objects
	char downloadUrl[60];
	char fileTemp[30];
	char currentUrl[35];
	// url to get html
	sprintf(currentUrl, "https://nhentai.net/g/%s/1/", doujin);
	// temporary
	puts(currentUrl);
	// write the html to our storedData array
	getHtml(currentUrl);
	// create the regex
	regcomp(&gereg, "https://i.nhentai.net/galleries/\\(.*\\)/[0-9].\\([a-z].*\\)\" w", 0);
	// execute the regex
	regexec(&gereg, storedData, 3, pmatch, 0);
	// 28 is a place holder, will add a get tag function
	for (int i = 1; i < 28; i++) {
		// currently not downloading the doujin; I don't have the number of pages
		// this is temporary
		sprintf(downloadUrl, "https://i.nhentai.net/galleries/%.*s/%d.%.*s", GALLERYID, i, GALLERYEX);
		puts(downloadUrl);
	}
	// free the regex
	regfree(&gereg);
}

void main(int argc, const char **argv) {
	// 219076	nothing special
	// 165598	page 2 is a png (rest jpg); good for debugging

	if (argc < 2) {
		printf("NO ID SPECIFIED EXITING\n");
		exit(0);
	}
	doProcess(argv[1]);
	doProcess(argv[2]);
	// mkdir(argv[1], 0700);
}