#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char* d_name(ntags h) {
    // I want to redo this
    char*** ptr[7] = {&h.parodies, &h.characters, &h.tags, &h.artists, &h.groups, &h.languages, &h.categories};
    size_t bufsize = 128;
    char *d = (char *) malloc(bufsize);
    snprintf(d, bufsize, "%s", h.id);
    if (NAMING != 0) {
        size_t size = snprintf(d, bufsize, "%s_", h.id);
        for (int x = 0; x < h.sizes[(NAMING - 3)]; x++) {
            if ((x + 1) >= h.sizes[(NAMING - 3)]) {
                size += snprintf(d+size, bufsize, "%s", ptr[(NAMING - 3)][0][x]);
            } else {
                size += snprintf(d+size, bufsize, "%s_", ptr[(NAMING - 3)][0][x]);
            }
            
            if (size >= bufsize) {
                bufsize += 64;
                d = (char *) realloc(d, bufsize);
            }
        }
    }
    return d;
}

int nhentai_download(ntags h) {
    printf("%s\n", h.title);
    char* d = d_name(h);

    if (strlen(d) >= 75) {
        sprintf(d, "%.*s", 72, d_name(h));
        fprintf(stderr, "\033[0;35m%s...: Name too long; resizing\033[0m\n", d);
    }

    char* textfile = (char *) malloc((sizeof(d) + sizeof(h.id) + 1) * sizeof(char*));
    sprintf(textfile, "%s/%s.txt", d, h.id);

    if (mkdir(d, 0777) == -1) {
        fprintf(stderr, "\033[0;31m%s: Directory exists; skipping\033[0m", d);
        return -1;
    }

    FILE* output = fopen(textfile, "w");
    const char *ident[7] = {"Parodies", "Characters", "Tags", "Artists", "Groups", "Languages", "Categories"};
    char*** ptr[7] = {&h.parodies, &h.characters, &h.tags, &h.artists, &h.groups, &h.languages, &h.categories};
    fprintf(output, "Title: %s\n", h.title);
    fprintf(output, "Gallery-Id: %s\n", h.gallery_id);
    // for (int type = 0; type < 7; type++) {
    for (int type = 2; type < 7; type++) {
        fprintf(output, "%s: ", ident[type]);
        for (int z = 0; z < h.sizes[type]; z++) {
            if ((z + 1) >= h.sizes[type]) {
                fprintf(output, "%s", ptr[type][0][z]);
            } else {
                fprintf(output, "%s, ", ptr[type][0][z]);
            }
        }
        fprintf(output, "\n");
    }
    fprintf(output, "Pages: %d", h.pages);
    fclose(output);

    int i = 0;
    char* ext[] = {"jpg", "png", "gif"};

    pid_t pid;
    for (int p = 1; p <= h.pages; p++) {
        pid = fork();

        if (pid == 0) {
            while (download_gallery(h.gallery_id, p, d, ext[i]) == 0) {
                i++;
                if (i > 2) {
                    i = 0;
                }
            }
            exit(0);
        } else if (pid > 0) {
            // the child has been forked and we are in the parent 
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
            progress_bar(progess, h.pages);
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    // 219076   nothing special
    // 165598   page 2 is a png (rest jpg); good for debugging
    // 295107   huge amount of tags
    // 267595   weird issue
    // 209949   corrupted
    for (int i = 1; i < argc; i++) {
        printf("Downloading (%d/%d) %s : ", i, (argc - 1), argv[i]);
        nhentai_download(nhentai_tags(argv[i]));
        putchar('\n');
    }
    return 0;
} 
