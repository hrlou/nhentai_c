#include <stdio.h>
#include <regex.h>

#ifdef __FreeBSD__
#include <sys/stat.h>
#endif

#include "main.h"
#include "download.h"

// #define HOME getenv("HOME")

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

int main(int argc, char **argv) {
	// 298385
	// 219076	nothing special
	// 165598	page 2 is a png (rest jpg); good for debugging
	// https://i.nhentai.net/galleries/1075519/1.jpg
	if (argc < 2) {
		printf("NO ID SPECIFIED, EXITING\n");
	}
	for (int doujin = 1; doujin < argc; doujin++) {
		regmatch_t pmatch[2];
		printf("Downloading (%d/%d) %s\n", doujin, (argc - 1), argv[doujin]);
		char currentUrl[35];
		sprintf(currentUrl, "https://nhentai.net/g/%s/", argv[doujin]);
		getHtml(currentUrl);
		regex_t preg;
		regcomp(&preg, "galleries/\\(.*\\)/cover.*\" /><meta property=\"og:type\".*Pages:.*>>>\\([0-9].*\\)>>>>>.*Uploaded", 0);
		regexec(&preg, storedData, 3, pmatch, 0);
		regfree(&preg);
		
		char galleryId[(pmatch[1].rm_eo - pmatch[1].rm_so)];
		sprintf(galleryId, "%.*s", pmatch[1].rm_eo - pmatch[1].rm_so, &storedData[pmatch[1].rm_so]);

		char pageNumber[(pmatch[2].rm_eo - pmatch[2].rm_so)];
		sprintf(pageNumber, "%.*s", pmatch[2].rm_eo - pmatch[2].rm_so, &storedData[pmatch[2].rm_so]);

		mkdir(argv[doujin], 0700);

		static int exI = 0;
		static char* extensions[] = { "jpg", "png", "gif" };
		for (int i = 1; i <= atoi(pageNumber); i++) {
			while (doDownload(galleryId, i, argv[doujin], extensions[exI]) == 0) {
				exI++;
				if (exI > 2) {
					exI = 0;
				}
			}
			progressBar(i, atoi(pageNumber));
		}
		putchar('\n');
	}
	return 0;
}