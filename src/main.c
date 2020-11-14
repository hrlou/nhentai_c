#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __FreeBSD__
#include <sys/stat.h>
#endif

#include "main.h"
#include "download.h"
#include "../config.def.h"
#include "global.h"

char storedData[15000];

void progressBar(float num, float den) {
    int index = 1;
    float perc = ((num / den) * 100);
    printf("\r(%0.0f%%) [%s", perc, STATUS_COLOUR);
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

char *getTags(char* id, char* galleryId, char* pages) {
    char storedTags[10][25][100];
    char *dataTypes[] = {"Title", "Gallery-Id", "Parodies", "Characters", "Tags", "Artists", "Groups", "Languages", "Categories", "Pages"};
    int tagSizes[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    int c = 0;
    int tagType = 0;
    // puts(storedData);
    while (storedData[c] != '\0') {
        if (storedData[c] == 'm' && storedData[c + 5] == 'i' && storedData[c + 15] == 'n' && storedData[c + 21] == 'c') {
            // TITLE
            c += 30;
            uint8_t i = 0;
            while (storedData[c + 2] != '/') {
                storedTags[tagType][0][i] = storedData[c];
                c++;
                i++;
            }
            storedTags[tagType][0][i] = '\0';
            puts(storedTags[tagType][0]);
            tagSizes[tagType] = 1;
            tagType++;
        } else if (storedData[c] == 'm' && storedData[c + 29] == '=' && storedData[c + 53] == 'g' && storedData[c + 61] == 's') {
            // GALLERY ID
            // <meta-itemprop="image"-content="https://t.nhentai.net/galleries/941176/cover.jpg"-/>
            c+=63;
            uint8_t i = 0;
            while (storedData[c] != '/') {
                storedTags[tagType][0][i] = storedData[c];
                c++;
                i++;
            }
            storedTags[tagType][0][i] = '\0';
            tagSizes[tagType] = 1;
            tagType++;
        } else if (storedData[c] == 'P' && storedData[c + 2] == 'r' && storedData[c + 8] == ':') {
            /*Parodies:
            >>>princess-connect>>314>>>>>*/
            uint8_t lineCount = 0;
            while (lineCount < 15) {
                do { c++; } while (storedData[c - 1] != '\n');
                lineCount++;

                uint8_t tagStoreCount = 0;
                uint8_t testIterate = 0;
                while (storedData[c] != '\n') {
                    // get past the >
                    while (storedData[c] == '>') {
                        c++;
                    }
                    // if we are saving
                    if (storedData[c] != '>' && storedData[c] != '\n' && testIterate % 2 == 0) {
                        uint8_t i = 0;
                        while (storedData[c] != '>' && storedData[c] != '\n') {
                            // putchar(storedData[c]);
                            storedTags[tagType][tagStoreCount][i] = storedData[c];
                            c++;
                            i++;
                        }
                        storedTags[tagType][tagStoreCount][i] = '\0';
                        tagSizes[tagType] = (tagStoreCount + 1);
                        // printf("storedTags %d %d    %s\n", tagType, tagSizes[tagType], storedTags[tagType][tagStoreCount]);
                        tagStoreCount++;
                    } else {
                        // skip
                        while (storedData[c] != '>' && storedData[c] != '\n') {
                            c++;
                        }
                    }
                    // finish
                    if (storedData[c] == '>') {
                        testIterate++;
                    }
                }
                c++;
                lineCount++;
                tagType++;
            }
        }
        c++;
    }
    // make the directory name
    // this was erroring because sometimes size is becoming too big, I'm unsure if this is still a problem since the bufsize is so large
    size_t bufsize = 128;
    char *directory = (char *) malloc(bufsize);
    if (NAMING != 0) {
        size_t size = snprintf(directory, bufsize, "%s_", id);
        // size++;
        for (uint8_t x = 0; x < tagSizes[(NAMING - 1)]; x++) {
            // puts(storedTags[(NAMING - 1)][x]);
            // printf("%ld\n", size);
            if ((x + 1) >= tagSizes[(NAMING - 1)]) {
                size += snprintf(directory+size, bufsize, "%s", storedTags[(NAMING - 1)][x]);
            } else {
                size += snprintf(directory+size, bufsize, "%s_", storedTags[(NAMING - 1)][x]);
            }
            
            if (size >= bufsize) {
                bufsize += 64;
                directory = (char *) realloc(directory, bufsize);
            }
        }
    } else {
        char *directory = (char *) malloc(20);
	snprintf(directory, 20, "%s\0", id);
	// broken for some odd reason
	puts(directory);
    }

    mkdir(directory, 0700);

    char *tagsFile = (char*) malloc(bufsize + 15);
    snprintf(tagsFile, (bufsize + 15), "%s/%s.txt", directory, id);

    // write tags to file
    FILE *fileWrite;
    fileWrite = fopen(tagsFile, "w");
    free(tagsFile);
    for (uint8_t y = 0; y < 10; y++) {
        fprintf(fileWrite, "%s: ", dataTypes[y]);
        for (uint8_t x = 0; x < tagSizes[y]; x++) {
            if ((x + 1) >= tagSizes[y]) {
                fprintf(fileWrite, "%s", storedTags[y][x]);
            } else {
                fprintf(fileWrite, "%s, ", storedTags[y][x]);
            }
        }
        fprintf(fileWrite, "\n");
    }
    fclose(fileWrite);

    /*for (uint8_t y = 0; y < 10; y++) {
        printf("%s: ", dataTypes[y]);
        for (uint8_t x = 0; x < tagSizes[y]; x++) {
            if ((x + 1) >= tagSizes[y]) {
                printf("%s", storedTags[y][x]);
            } else {
                printf("%s, ", storedTags[y][x]);
            }
        }
        printf("\n");
    }*/

    snprintf(galleryId, 10, "%s", storedTags[1][0]);
    snprintf(pages, 5, "%s", storedTags[9][0]);
    return directory;
}

int main(int argc, char **argv) {
    // 219076	nothing special
    // 165598	page 2 is a png (rest jpg); good for debugging
    // 295107	huge amount of tags
    // 267595   weird issue
    if (argc < 2) {
        printf("NO ID SPECIFIED, EXITING\n");
    }
    for (int doujin = 1; doujin < argc; doujin++) {
        printf("Downloading (%d/%d) %s : ", doujin, (argc - 1), argv[doujin]);
        char *currentUrl = (char *) malloc(35);
        sprintf(currentUrl, "https://nhentai.net/g/%s/", argv[doujin]);
        getHtml(currentUrl);
        free(currentUrl);
        // puts(storedData);

        char *galleryId = (char *) malloc(10);
        char *pageCount = (char *) malloc(5);
        char *directory = getTags(argv[doujin], galleryId, pageCount);
        // puts(galleryId);
        // puts(pageCount);
        // puts(directory);
        
        int exI = 0;
        char* extensions[] = {"jpg", "png", "gif"};
        for (uint8_t page = 1; page <= atoi(pageCount); page++) {
            while (doDownload(galleryId, page, directory, extensions[exI]) == 0) {
                exI++;
                if (exI > 2) {
                    exI = 0;
                }
            }
            progressBar(page, atoi(pageCount));
        }
        putchar('\n');
    }
    return 0;
} 
