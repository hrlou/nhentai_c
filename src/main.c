#include <stdio.h>

#ifdef __FreeBSD__
#include <sys/stat.h>
#endif

#include "main.h"
#include "download.h"
#include "../config.def.h"

#include <stdlib.h>
#include <string.h>

typedef enum {true, false} bool;

void progressBar(float num, float den) {
    int index = 1;
    float perc = ((num / den) * 100);
    printf("\r(%0.0f%) [%s", STATUS_COLOUR, perc);
    for (float i = 0; i <= STATUS_SIZE; i++) {
        if (i <= (perc / (100 / STATUS_SIZE))) {
        	putchar(STATUS_SYMBOL);
        } else {
        	putchar(' ');
        }
    }
    printf("\033[0m] (%0.0f/%0.0f)", num, den);
    fflush(stdout);
}

void getTags(char* id, char* directory, char* gid, char* pages) {
	/* 	0	Title: Shiki-nyan-wa-Producer-de-Lotion-Onanie-ga-Yamerarenai!
		1	Gallery-Id: 1163557
		2	Parodies: the-idolmaster, 
		3	Characters: producer, shiki-ichinose, 
		4	Tags: sole-female, sole-male, big-ass, footjob, 
		5	Artists: saki-chisuzu, 
		6	Groups: berry-16, 
		7	Languages: japanese, 
		8	Categories: doujinshi, 
		9	Pages: 28,	*/

	// printf("tag = %s\n", tagStore[3][1]);

	char *file = malloc(12 * sizeof(char*));

	snprintf(file, 12, "%s.txt", id);
	snprintf(directory, (strlen(id) + 2), "%s_", id);

	bool namingScheme = false;
	bool getId = true;
	bool getPages = false;

	FILE *fileWrite;
    fileWrite = fopen(file, "w");

    snprintf(gid, strlen(gid), "\0");
    snprintf(pages, strlen(pages), "\0");

    size_t c = 0;
    size_t di = 0;

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
		// GALLERY ID
		// galleries/\\(.*\\)/cover.*\"-/><meta-property=\"og:type\"
		if (storedData[c] == 'g' && storedData[c + 1] == 'a' && storedData[c + 8] == 's' && getId == true) {
			c+=10;
			fprintf(fileWrite, "Gallery-Id: ");
			while (storedData[c] != '/') {
				fprintf(fileWrite, "%c", storedData[c]);
				sprintf(gid, "%s%c", gid, storedData[c]);
				c++;
			}
			fprintf(fileWrite, "\n");
			getId = false;
		}
		// TAGS
		// start if charecter is a new line and next charecter is a capital and not U; avoiding the uploaded class
		if (storedData[c] == '\n' && storedData[c + 1] >= 65 && storedData[c + 1] <= 90 && storedData[c + 1] != 'U') {
			c++;
			// for naming scheme
			if (storedData[c] == NAMING[0] && storedData[c + 1] == NAMING[1] && storedData[c + 2] == NAMING[2]) {
				namingScheme = true;
			}
			if (storedData[c] == 'P' && storedData[c + 1] == 'a' && storedData[c + 2] == 'g') {
				getPages = true;
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
					if (getPages == true) {
						sprintf(pages, "%s%c", pages, storedData[c]);
					}
					if (namingScheme == true) {
						sprintf(directory, "%s%c", directory, storedData[c]);
					}

					fprintf(fileWrite, "%c", storedData[c]);
					if (storedData[c + 1] == '>') {
						if (getPages == true) {
							sprintf(pages, "%s\0", pages);
						}
						if (namingScheme == true) {
							sprintf(directory, "%s_", directory);
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
		getPages = false;
	}

	fclose(fileWrite);
	int i = 0;
	while (directory[i + 1] != '\0') {
		i++;
	}
	directory[i] = '\0';

	char *new = malloc(75 * sizeof(char*));
	sprintf(new, "%s/%s", directory, file);
	mkdir(directory, 0700);
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
		printf("Downloading (%d/%d) %s : ", doujin, (argc - 1), argv[doujin]);

		char *currentUrl = malloc(35 * sizeof(char*));
		sprintf(currentUrl, "https://nhentai.net/g/%s/", argv[doujin]);
		getHtml(currentUrl);
		free(currentUrl);

		char *directory = malloc(75 * sizeof(char));
		char *galleryId = malloc(10 * sizeof(char));
		char *pageNumber = malloc(4 * sizeof(char));

		getTags(argv[doujin], directory, galleryId, pageNumber);

		static int exI = 0;
		static char* extensions[] = {"jpg", "png", "gif"};
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