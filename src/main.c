#include <stdio.h>
#include <regex.h>

#ifdef __FreeBSD__
#include <sys/stat.h>
#endif

#include "main.h"
#include "download.h"
#include "../config.def.h"

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

void getTags(char* id, char* name) {
	char file[15];
	sprintf(file, "%s.txt", id);

	sprintf(name, "%s_", id);
	bool namingScheme = false;

	FILE *fileWrite;
    fileWrite = fopen(file, "w");

    int c = 0;
	while (storedData[c] != '\0') {
		// TITLE
		if (storedData[c] == 'm' && storedData[c + 5] == 'i' && storedData[c + 15] == 'n' && storedData[c + 21] == 'c') {
			c += 30;
			fprintf(fileWrite, "Title: ");
			while (storedData[c] != '\"') {
				putchar(storedData[c]);
				fprintf(fileWrite, "%c", storedData[c]);
				c++;
			}
			putchar('\n');
			fprintf(fileWrite, "\n");
		}
		c++;
		// TAGS
		// start if charecter is a new line and next charecter is a capital and not U; avoiding the uploaded class
		if (storedData[c] == '\n' && storedData[c + 1] >= 65 && storedData[c + 1] <= 90 && storedData[c + 1] != 'U') {
			c++;
			// for naming scheme
			if (storedData[c] == NAMING[0] && storedData[c + 1] == NAMING[1] && storedData[c + 2] == NAMING[2]) {
				namingScheme = true;
			}

			// print the tag class, i.e Parodies:
			while (storedData[c] != '\n') {
				fprintf(fileWrite, "%c", storedData[c]);
				c++;
			}
			// formating
			fprintf(fileWrite, " ");
			c++;
			int test = 0;
			while (storedData[c + 1] != '\n') {
				// iterate the amount of >
				if (storedData[c] == '>') {
					test++;
				}
				// there are only 2 > infront of the amount of tags; don't print that
				while (storedData[c] != '>' && test >= 3) {
					if (namingScheme == true) {
						sprintf(name, "%s%c", name, storedData[c]);
					}
					fprintf(fileWrite, "%c", storedData[c]);
					if (storedData[c + 1] == '>') {
						if (namingScheme == true) {
							sprintf(name, "%s_", name);
						}
						fprintf(fileWrite, ", ");
						test = 0;
					}
					c++;
				}
				c++;
			}
			fprintf(fileWrite, "\n");
		}
		namingScheme = false;
	}
	fclose(fileWrite);
	int i = 0;
	while (name[i + 1] != '\0') {
		i++;
	}
	name[i] = '\0';

	char new[75];
	sprintf(new, "%s/%s", name, file);
	mkdir(name, 0700);
	rename(file, new);
}

int main(int argc, char **argv) {
	// 219076	nothing special
	// 165598	page 2 is a png (rest jpg); good for debugging
	// https://i.nhentai.net/galleries/1075519/1.jpg
	if (argc < 2) {
		printf("NO ID SPECIFIED, EXITING\n");
	}
	for (int doujin = 1; doujin < argc; doujin++) {
		regmatch_t pmatch[2];
		regex_t preg;
		printf("Downloading (%d/%d) %s : ", doujin, (argc - 1), argv[doujin]);
		char currentUrl[35];
		sprintf(currentUrl, "https://nhentai.net/g/%s/", argv[doujin]);
		getHtml(currentUrl);
		regcomp(&preg, "galleries/\\(.*\\)/cover.*\"-/><meta-property=\"og:type\".*Pages:.*>>>\\([0-9].*\\)>>>>>.*Uploaded", 0);
		regexec(&preg, storedData, 3, pmatch, 0);
		regfree(&preg);
		
		char galleryId[(pmatch[1].rm_eo - pmatch[1].rm_so)];
		sprintf(galleryId, "%.*s", pmatch[1].rm_eo - pmatch[1].rm_so, &storedData[pmatch[1].rm_so]);

		char pageNumber[(pmatch[2].rm_eo - pmatch[2].rm_so)];
		sprintf(pageNumber, "%.*s", pmatch[2].rm_eo - pmatch[2].rm_so, &storedData[pmatch[2].rm_so]);

		char directory[LIMIT];
		getTags(argv[doujin], directory);

		static int exI = 0;
		static char* extensions[] = { "jpg", "png", "gif" };
		for (int i = 1; i <= atoi(pageNumber); i++) {
			while (doDownload(galleryId, i, directory, extensions[exI]) == 0) {
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