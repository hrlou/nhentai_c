#include <stdio.h>
#include <regex.h>
#include <stdlib.h>

#define HOME getenv("HOME")

#include "main.h"
#include "download.h"

// can't declare regex variables in the scope when using loops?
regmatch_t pmatch[2];
regex_t preg;

void progressBar(float num, float den) {
    int index = 1;
    float perc = ((num / den) * 100);
    printf("\r(%0.0f%) [", perc);
    for (float i = 0; i <= 50; i++) {
        if (i <= (perc / 2)) {
        	putchar('#');
        } else {
        	putchar(' ');
        }
    }
    printf("] (%0.0f/%0.0f)", num, den);
    fflush(stdout);
}

void doProcess(char* doujin) {
	// extensions array, some doujins have multiple extensions
	char *extensions[] = {"jpg", "png", "gif"};
	// store the current url
	char currentUrl[35];
	// set it to the page
	sprintf(currentUrl, "https://nhentai.net/g/%s/", doujin);
	// download
	getHtml(currentUrl);
	// puts(storedData);
	// regex
	// regcomp(&preg, "galleries/\\(.*\\)/cover.*\" /><meta property=\"og:type\".*<span class=\"name\">\\([0-9].*\\)<\\/span></a></span></div>", 0);
	regcomp(&preg, "galleries/\\(.*\\)/cover.*\" /><meta property=\"og:type\".*Pages:.*>>>\\([0-9].*\\)>>>>>.*Uploaded", 0);
	regexec(&preg, storedData, 3, pmatch, 0);
	regfree(&preg);
	// useful debbuging
	// printf("%d=%d\n", pmatch[1].rm_eo, pmatch[1].rm_so);
	// printf("%.*s\n", pmatch[1].rm_eo - pmatch[1].rm_so, &storedData[pmatch[1].rm_so]);
	// printf("%.*s\n", pmatch[2].rm_eo - pmatch[2].rm_so, &storedData[pmatch[2].rm_so]);
	char pageNumber[4];
	sprintf(pageNumber, "%.*s", pmatch[2].rm_eo - pmatch[2].rm_so, &storedData[pmatch[2].rm_so]);
	char galleryId[8];
	sprintf(galleryId, "%.*s", pmatch[1].rm_eo - pmatch[1].rm_so, &storedData[pmatch[1].rm_so]);
	mkdir(doujin, 0700);
	int exi = 0;
	for (int i = 1; i <= atoi(pageNumber); i++) {
		while (doDownload(galleryId, i, doujin, extensions[exi]) == 0) {
			exi++;
			if (exi > 1) {
				exi = 0;
			}
		}
		progressBar(i, atoi(pageNumber));
	}
}

void main(int argc, char **argv) {
	// 298385
	// 219076	nothing special
	// 165598	page 2 is a png (rest jpg); good for debugging
	// https://i.nhentai.net/galleries/1075519/1.jpg
	if (argc < 2) {
		printf("NO ID SPECIFIED, EXITING\n");
	}
	for (int i = 1; i < argc; i++) {
		printf("Downloading (%d/%d) %s\n", i, (argc - 1), argv[i]);
		doProcess(argv[i]);
		putchar('\n');
	}
	// mkdir(argv[1], 0700);
}