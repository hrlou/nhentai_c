#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "main.h"
#include "download.h"
#include "parse.h"
#include "search.h"
#include "../config.def.h"

void progress_bar(float numerator, float denominator) {
    int index = 1;
    float percent = ((numerator / denominator) * 100);
    printf("\r(%0.0f%%) [%s", percent, STATUS_COLOUR);
    for (float i = 0; i <= STATUS_SIZE; i++) {
        if (i <= (percent / (100 / STATUS_SIZE))) {
            putchar(STATUS_SYMBOL);
        } else {
            putchar(' ');
        }
    }
    printf("\033[0m] (%0.0f/%0.0f)", numerator, denominator);
    fflush(stdout);
}

void doujin_download(char* doujin) {
    char *current_url = (char *) malloc(35);
    sprintf(current_url, "https://nhentai.net/g/%s/", doujin);
    get_html(current_url, 0);
    free(current_url);
    // puts(stored_data);

    char *gallery_id = (char *) malloc(10);
    char *page_count = (char *) malloc(5);
    char *directory = parse_tags(doujin, gallery_id, page_count);
    // puts(galleryId);
    // puts(pageCount);
    // puts(directory);
    
    int extension_iterate = 0;
    char* extension_types[] = {"jpg", "png", "gif"};
    pid_t pid = fork();
    int pages = atoi(page_count);
    int j;
    if (pages % 2 == 0) {
        j = (pages / 2);
    } else {
        j = ((pages / 2) + 1);
    }
    // child process
    if (pid==0) {
        for (int page_current = 1; page_current <= j; page_current++) {
            while (download_gallery(gallery_id, page_current, directory, extension_types[extension_iterate]) == 0) {
                extension_iterate++;
                if (extension_iterate > 2) {
                    extension_iterate = 0;
                }
            }
            // progress_bar(page_current, pages);
        }
    } else {
        int counter = 0;
        for (int page_current = (j + 1); page_current <= pages; page_current++) {
            while (download_gallery(gallery_id, page_current, directory, extension_types[extension_iterate]) == 0) {
                extension_iterate++;
                if (extension_iterate > 2) {
                    extension_iterate = 0;
                }
            }
            counter+=2;
            progress_bar(counter, pages);
        }
        waitpid(pid,0,0);
        progress_bar(pages, pages);
    }
    // curl_global_cleanup();

    /*
     for (int page_current = 1; page_current <= atoi(page_count); page_current++) {
            while (download_gallery(gallery_id, page_current, directory, extension_types[extension_iterate]) == 0) {
                extension_iterate++;
                if (extension_iterate > 2) {
                    extension_iterate = 0;
                }
            }
            progress_bar(page_current, atoi(page_count));
        }
        */
}

int main(int argc, char **argv) {
    // 219076	nothing special
    // 165598	page 2 is a png (rest jpg); good for debugging
    // 295107	huge amount of tags
    // 267595   weird issue
    if (argc < 2) {
        printf("Search is Currently just a demo!\n");
        search_menu();
        exit(0);
    }
    for (int doujin_iterate = 1; doujin_iterate < argc; doujin_iterate++) {
        printf("Downloading (%d/%d) %s : ", doujin_iterate, (argc - 1), argv[doujin_iterate]);
        doujin_download(argv[doujin_iterate]);
        putchar('\n');  
    }
    return 0;
} 
