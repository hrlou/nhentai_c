#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

#include "main.h"
#include "nhentai.h"
#include "cbz.h"
#include "output.h"
#include "curl.h"
#include "search.h"

#include "../config.def.h"

/* I will construct an options struct for all of the handle functions to make things easier, for now it's just like this */

static void handle_nhentai(nhentai_T* nhentai) {
    nhentai_download(nhentai);
    if (CREATE_CBZ) {
        nhentai_cbz(nhentai);
    }
    if (CREATE_CBZ && DELETE_DIR) {
        remove_recursive(nhentai->dir);
    }
    free(nhentai);
}

static void handle_nhentai_array(nhentai_T** nhentai, int count) {
    for (int i = 0; i < count; i++) {
        if (nhentai[i]->tags == NULL) {
            nhentai_data(nhentai[i]);
        }
        fprintf(stderr, "Downloading (%d/%d) %s : %s\n", i + 1, count, nhentai[i]->id, nhentai[i]->tags->title);
        handle_nhentai(nhentai[i]);
    }
}

static void handle_search_inspect(search_T* search) {
    for (int i = 0; i < search->count; i++) {
        nhentai_data(search->results[i]);
        fprintf(stderr, "(%d/%ld) %s\n", i + 1, search->count, search->results[i]->id);
        ntags_print(stderr, search->results[i]->tags);
        putc('\n', stderr);
        for (;;) {
            fprintf(stderr, "Download [Y/N]: ");
            char opt = toupper(getchar());
            getchar();
            if (opt == 'Y') {
                handle_nhentai(search->results[i]);
                break;
            } else if (opt == 'N') {
                break;
            } else {
                fputs("Invalid Argument\n", stderr);
            }
        }
        putc('\n', stderr);
    }
}

static void handle_search(search_T* search) {
    fprintf(stderr, "%ld results for \"%s\"\n", search->count, search->term);
    for (;;) {
        fprintf(stderr, "\tA. Download All\n\tI. Inspect All\n\tC. Cancel\n[A/I/C]: ");
        char opt = toupper(getchar());
        getchar();
        switch (opt) {
            case 'A': return handle_nhentai_array(search->results, search->count);
            case 'I': return handle_search_inspect(search);
            case 'C': return;
            default: fputs("Invalid Argument", stderr);
        }
    }
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
        handle_search(arg_search);
    }

    /* handle ids (arguments not attached to options) */
    int dcount = 0;
    nhentai_T** arg_nhentai = calloc(1, sizeof(struct nhentai**));
    for (int index = optind; index < argc; index++) {
        arg_nhentai = realloc(arg_nhentai, (1 + dcount) * sizeof(struct nhentai*));
        arg_nhentai[dcount++] = nhentai_init(argv[index]);
    }
    handle_nhentai_array(arg_nhentai, dcount);
    return 0;
}

    