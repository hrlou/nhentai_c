#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <string.h>

#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ftw.h>

#include "output.h"
#include "nhentai.h"
#include "ntags.h"
#include "../config.def.h"

static int unlink_cb(const char* fpath, const struct stat* sb, int typeflag, struct FTW* ftwbuf) {
    int rv = remove(fpath);
    if (rv) {
        perror(fpath);
    }
    return rv;
}

int remove_recursive(char* path) {
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

static char* visual_list(char** list, size_t list_size) {
    if (list_size == 0) {
        return NULL;
    }
    size_t length = (2 + strlen(list[0]));
    char* vlist = calloc(length, sizeof(char));
    snprintf(vlist, length, "%s_", list[0]);
    for (size_t i = 1; i < list_size; i++) {
        vlist = realloc(vlist, (2 + length + strlen(list[i])) * sizeof(char));
        length += snprintf(vlist+length-1, (2 + length + strlen(list[i])), "%s_", list[i]); // lgtm [cpp/overflowing-snprintf]
    }
    vlist[strlen(vlist) - 1] = '\0';
    return vlist;
}

static int arrncmp(const char* str, const char** arr, int length) {
    for (size_t i = 0; arr[i]; i++) {
        if (strncmp(str, arr[i], length) == 0) {
            return i + 1;
        }
    }
    return 0;
}

static void progress_bar(float numerator, float denominator) {
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

static int download_page(nhentai_T* nhentai, int index, int extension) {
    const char* ext[] = {"jpg", "png", "gif"};
    char* url = calloc((42 + strlen(nhentai->tags->gallery_id)), sizeof(char));
    char* file = calloc((30 + strlen(nhentai->dir)), sizeof(char));
    snprintf(url, (42 + strlen(nhentai->tags->gallery_id)), "https://i.nhentai.net/galleries/%s/%d.%s", nhentai->tags->gallery_id, index, ext[extension]);
    snprintf(file, (30 + strlen(nhentai->dir)),  "%s/%03d.%s", nhentai->dir, index, ext[extension]);
    int value = curl_download_file(url, file);
    free(url);
    free(file);
    return value;
}

void directory_gen(nhentai_T* nhentai) { 
    const char* variables[] = {"id", "title", "gallery", "parodies", "characters", "tags", "artists", "groups", "language", NULL};
    char*** ptr[8] = {&nhentai->tags->parodies, &nhentai->tags->characters, &nhentai->tags->tags, &nhentai->tags->artists, &nhentai->tags->groups, &nhentai->tags->languages, &nhentai->tags->categories};

    nhentai->dir = calloc(MAX_DIRECTORY_LENGTH + 8, sizeof(char));
    char* name = NAMING;
    size_t size = 0;

    for (; *name; name++) {
        if (*name == '$') {
            name++;
            int r = arrncmp(name, variables, (strchr(name, '$') - name));
            if (r != 0) {
                r -= 1;
                char* tmp = NULL;
                if (r <= 2) {
                    switch (r) {
                        case 0: tmp = nhentai->id; break;
                        case 1: tmp = nhentai->tags->title; break;
                        case 2: tmp = nhentai->tags->gallery_id; break;
                    }
                } else {
                    tmp = visual_list(*ptr[r - 3], nhentai->tags->sizes[r - 3]);
                }
                if (tmp != NULL) {
                    strncat(nhentai->dir, tmp, (MAX_DIRECTORY_LENGTH - size));
                    size += (strlen(tmp) > (MAX_DIRECTORY_LENGTH - size)) ? (MAX_DIRECTORY_LENGTH - size) : strlen(tmp);
                    name = strchr(name, '$');
                }
            }
        } else {
            if (size < MAX_DIRECTORY_LENGTH) {
                nhentai->dir[size++] = *name;
            }
        }
    }
}

void download_gallery(nhentai_T* nhentai) {
    directory_gen(nhentai);
    mkdir(nhentai->dir, 0755);
    char* textfile = calloc((20 + strlen(nhentai->dir)), sizeof(char));
    snprintf(textfile, (20 + strlen(nhentai->dir)), "%s/%s.txt", nhentai->dir, nhentai->id);
    FILE* output = fopen(textfile, "w");
    free(textfile);
    ntags_print(output, nhentai->tags);
    fclose(output);

    pid_t pid;
    
    for (int p = 1, i = 0; p <= nhentai->tags->pages; p++) {
        pid = fork();
        if (pid == 0) {
            while (!download_page(nhentai, p, i)) {
                i++;
                if (i > 2) {
                    i = 0;
                }
            }
            exit(0);
        } else if (pid > 0) {
            /* the child has been forked and we are in the parent */
        } else {
            perror("FAILED TO FORK");
        }
    }

    int status, progess = 0;

    while ((pid = waitpid(-1, &status, 0))) {
        if (pid == -1) {
            break;
        } else if (pid == -1) {
            perror("waitpid");
        } else if (WIFEXITED(status)) {
            /* printf("%d exited, status=%d\n", pid, WEXITSTATUS(status)); */
            progess++;
            progress_bar(progess, nhentai->tags->pages);
        }
    }
    putchar('\n');
}