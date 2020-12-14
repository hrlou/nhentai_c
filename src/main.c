#include <stdio.h>
#include <stdlib.h>

#include <sys/wait.h>
#include <unistd.h>

#ifdef __FreeBSD__
#include <sys/stat.h>
#endif

#include "main.h"

void progress_bar(float numerator, float denominator) {
    float percent = ((numerator / denominator) * 100);
    fprintf(stderr, "\r(%0.0f%%) [%s", percent, STATUS_COLOUR);
    for (float i = 0; i <= STATUS_SIZE; i++) {
        if (i <= (percent / (100 / STATUS_SIZE))) {
            putc(STATUS_SYMBOL, stderr);
        } else {
            putc(' ', stderr);
        }
    }
    fprintf(stderr, "\033[0m] (%0.0f/%0.0f)", numerator, denominator);
}

char* directory_name(ntags nhentai) {
    char*** tags_point[7] = {&nhentai.parodies, &nhentai.characters, &nhentai.tags, &nhentai.artists, &nhentai.groups, &nhentai.languages, &nhentai.categories};
    size_t bufsize = 128;
    char *directory = (char *) malloc(bufsize);
    snprintf(directory, bufsize, "%s", nhentai.id);
    if (NAMING != 0) {
        size_t size = snprintf(directory, bufsize, "%s_", nhentai.id);
        for (int x = 0; x < nhentai.sizes[(NAMING - 3)]; x++) {
            if ((x + 1) >= nhentai.sizes[(NAMING - 3)]) {
                size += snprintf(directory+size, bufsize, "%s", tags_point[(NAMING - 3)][0][x]);
            } else {
                size += snprintf(directory+size, bufsize, "%s_", tags_point[(NAMING - 3)][0][x]);
            }
            
            if (size >= bufsize) {
                bufsize += 64;
                directory = (char *) realloc(directory, bufsize);
            }
        }
    }
    return directory;
}

void nhentai_download(ntags nhentai) {
    // putchar('\n');
    printf("%s\n", nhentai.title);
    char* directory = directory_name(nhentai);
    // char* directory = "test";
    char* textfile = (char *) malloc((sizeof(directory) + sizeof(nhentai.id) + 1) * sizeof(char*));
    sprintf(textfile, "%s/%s.txt", directory, nhentai.id);
    mkdir(directory, 0777);
    // write_ntags(nhentai, textfile);
    FILE* output;
    output = fopen(textfile, "w");
    // output = stderr;
    const char *tag_types[7] = {"Parodies", "Characters", "Tags", "Artists", "Groups", "Languages", "Categories"};
    char*** tags_point[7] = {&nhentai.parodies, &nhentai.characters, &nhentai.tags, &nhentai.artists, &nhentai.groups, &nhentai.languages, &nhentai.categories};
    fprintf(output, "Title: %s\n", nhentai.title);
    fprintf(output, "Gallery-Id: %s\n", nhentai.gallery_id);
    for (int type = 0; type < 7; type++) {
        fprintf(output, "%s: ", tag_types[type]);
        for (int z = 0; z < nhentai.sizes[type]; z++) {
            if ((z + 1) >= nhentai.sizes[type]) {
                fprintf(output, "%s", tags_point[type][0][z]);
            } else {
                fprintf(output, "%s, ", tags_point[type][0][z]);
            }
        }
        fprintf(output, "\n");
    }
    fprintf(output, "Pages: %d", nhentai.pages);
    // exit(0);
    fclose(output);

    int extension_iterate = 0;
    char* extension_types[] = {"jpg", "png", "gif"};

    pid_t pid;
    for (int page_current = 1; page_current <= nhentai.pages; page_current++) {
        pid = fork();

        if (pid == 0) {
            while (download_gallery(nhentai.gallery_id, page_current, directory, extension_types[extension_iterate]) == 0) {
                extension_iterate++;
                if (extension_iterate > 2) {
                    extension_iterate = 0;
                }
            }
            exit(0);
        } else if (pid > 0) {
            /* the child has been forked and we are in the parent */
        } else {
            perror("FAILED TO FORK");
        }
    }

    int status;
    int progess = 0;

    while ((pid = waitpid(-1, &status, 0))) {
        // if (pid == -1 && errno == ECHILD) {
        if (pid == -1) {
            break;
        } else if (pid == -1) {
            perror("waitpid");
        } else if (WIFEXITED(status)) {
            // printf("%d exited, status=%d\n", pid, WEXITSTATUS(status));
            progess++;
            progress_bar(progess, nhentai.pages);
        }
    }
}

int main(int argc, char **argv) {
    // 219076   nothing special
    // 165598   page 2 is a png (rest jpg); good for debugging
    // 295107   huge amount of tags
    // 267595   weird issue
    for (int doujin_iterate = 1; doujin_iterate < argc; doujin_iterate++) {
        printf("Downloading (%d/%d) %s : ", doujin_iterate, (argc - 1), argv[doujin_iterate]);
        nhentai_download(nhentai_tags(argv[doujin_iterate]));
        putchar('\n');
        // printf("\nTest\n");
    }
    return 0;
} 
