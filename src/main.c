#include <stdio.h>

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

void getTags(char* id, char* name, char* gid, char* pages) {
	char file[15];
	sprintf(file, "%s.txt", id);

	sprintf(name, "%s_", id);
	bool namingScheme = false;
	bool getId = true;
	bool getPages = false;

	FILE *fileWrite;
    fileWrite = fopen(file, "w");

    sprintf(gid, "\0");
    sprintf(pages, "\0");

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
						sprintf(name, "%s%c", name, storedData[c]);
					}

					fprintf(fileWrite, "%c", storedData[c]);
					if (storedData[c + 1] == '>') {
						if (getPages == true) {
							sprintf(pages, "%s\0", pages);
						}
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
		getPages = false;
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
		printf("Downloading (%d/%d) %s : ", doujin, (argc - 1), argv[doujin]);
		char currentUrl[35];
		sprintf(currentUrl, "https://nhentai.net/g/%s/", argv[doujin]);
		getHtml(currentUrl);

		char directory[LIMIT];
		char galleryId[10];
		char pageNumber[4];
		// pageNumber[0] = '\0';

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