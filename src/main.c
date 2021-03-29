#include <stdio.h>

#include "main.h"
#include "nhentai.h"
#include "cbz.h"
#include "output.h"
#include "curl.h"

#include "../config.def.h"

int main(int argc, char *argv[]) {
    int count = 1;
    for (; argv[count] != NULL; count++);
    for (int i = 1; i < count; i++) {
        nhentai_T* nhentai = nhentai_init(argv[i]);
        nhentai_data(nhentai);
        printf("Downloading (%d/%d) %s : %s\n", i, count - 1, nhentai->id, nhentai->tags->title);
        nhentai_download(nhentai);
        if (CREATE_CBZ) {
            nhentai_cbz(nhentai);
        }
        if (CREATE_CBZ && DELETE_DIR) {
            remove_recursive(nhentai->dir);
        }
        free(nhentai);
    }
    return 0;
}