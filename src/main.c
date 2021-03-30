#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "main.h"
#include "nhentai.h"
#include "cbz.h"
#include "output.h"
#include "curl.h"
#include "search.h"

#include "../config.def.h"

void nhentai_front(nhentai_T** nhentai, int count) {
    for (int i = 0; i < count; i++) {
        nhentai_data(nhentai[i]);
        printf("Downloading (%d/%d) %s : %s\n", i + 1, count, nhentai[i]->id, nhentai[i]->tags->title);
        nhentai_download(nhentai[i]);
        if (CREATE_CBZ) {
            nhentai_cbz(nhentai[i]);
        }
        if (CREATE_CBZ && DELETE_DIR) {
            remove_recursive(nhentai[i]->dir);
        }
    }
    free(nhentai);
}

int main(int argc, char *argv[]) {
    char* search_term = NULL;
    while (1) {
        int option_index = 0, c;
        static struct option long_options[] = {
            {"search",      required_argument,  0,  's'},
            {"help",        no_argument,        0,  'h'},
            {0,             0,                  0,  0}
        };

        c = getopt_long(argc, argv, "s:h", long_options, &option_index);
        if (c == -1) {
            break;
        }
        switch (c) {
            case 's':
                search_term = optarg;
                break;
            case 'h':
                puts("Help!");
                break;
            default:
                fprintf(stderr, "[getopt] returned character code 0 %d\n", c);
        }
    }

    /* handle search */
    if (search_term != NULL) {
        search_T* arg_search = nhentai_search(search_term);
        nhentai_front(arg_search->results, arg_search->count);
    }

    /* handle ids (arguments not attached to options) */
    int dcount = 0;
    nhentai_T** arg_nhentai = calloc(1, sizeof(struct nhentai**));
    for (int index = optind; index < argc; index++) {
        arg_nhentai = realloc(arg_nhentai, (1 + dcount) * sizeof(struct nhentai*));
        arg_nhentai[dcount++] = nhentai_init(argv[index]);
    }
    nhentai_front(arg_nhentai, dcount);
    return 0;
}

    