#include <stdio.h>

#include "main.h"
#include "nhentai.h"
#include "cbz.h"

int main(int argc, char *argv[]) {
    int count = 1;
    for (; argv[count] != NULL; count++);
    for (int i = 1; i < count; i++) {
        nhentai_T* nhentai = nhentai_init(argv[i]);
        nhentai_data(nhentai);
        printf("Downloading (%d/%d) %s : %s\n", i, count - 1, nhentai->id, nhentai->tags->title);
        /* I plan to just put the images directly into the zip later */
        nhentai_download(nhentai);
        nhentai_cbz(nhentai);
        free(nhentai);
    }
    return 0;
}