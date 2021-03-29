#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nhentai.h"

ntags_T* ntags_init(char* data) {
    ntags_T* ntags = calloc(1, sizeof(struct ntags));
    ntags->data = data;
    return ntags;
}

void ntags_print(FILE* fp, ntags_T* ntags) {
    const char* ident[7] = {"Parodies", "Characters", "Tags", "Artists", "Groups", "Languages", "Categories"};
    char*** ptr[7] = {&ntags->parodies, &ntags->characters, &ntags->tags, &ntags->artists, &ntags->groups, &ntags->languages, &ntags->categories};
    fprintf(fp, "Title: %s\n", ntags->title);
    fprintf(fp, "Gallery-Id: %s\n", ntags->gallery_id);
    for (int type = 2; type < 7; type++) {
        fprintf(fp, "%s: ", ident[type]);
        for (int z = 0; z < ntags->sizes[type]; z++) {
            if ((z + 1) >= ntags->sizes[type]) {
                fprintf(fp, "%s", ptr[type][0][z]);
            } else {
                fprintf(fp, "%s, ", ptr[type][0][z]);
            }
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "Pages: %d", ntags->pages);
}

void ntags_info(ntags_T* ntags) {
    char* s = ntags->data;
    for (; *s; s++) {
        if (*s == 'm' && *(s+5) == 'i' && *(s+15) == 'n' && *(s+21) == 'c') {
            /* meta-itemprop="name"-content=" */
            s += 30;
            int i = 0;
            for (; *(s + i + 2) != '/'; i++);
            ntags->title = calloc(i + 1, sizeof(char));
            strncpy(ntags->title, s, i);
            *(ntags->title+i) = 0;
        }
        if (*s == 'm' && *(s+29) == '=' && *(s+53) == 'g' && *(s+61) == 's') {
            /* <meta-itemprop="image"-content="https://t.nhentai.net/galleries/xxxxx/cover.jpg"-/> */
            s += 63;
            int i = 0;
            for (; *(s + i) != '/'; i++);
            ntags->gallery_id = calloc(i + 1, sizeof(char));
            strncpy(ntags->gallery_id, s, i);
            *(ntags->gallery_id+i) = 0;
        }
        if (*s == 'P' && *(s+6) == '\n') {
            s += 10;
            int i = 0;
            for (; *(s+i) != '>'; i++);
            char* pg;
            pg = calloc(i + 1, sizeof(char));
            strncpy(pg, s, i);
            *(pg + i) = 0;
            ntags->pages = atoi(pg);
            free(pg);
            break;
        }
    }
}

void ntags_tags(ntags_T* ntags) {
    char*** ptr[8] = {&ntags->parodies, &ntags->characters, &ntags->tags, &ntags->artists, &ntags->groups, &ntags->languages, &ntags->categories};
    char* s = strstr(ntags->data, "Parodies:");
    /* >>>xxxx-xxxx>>314>>>>> */
    for (int tt = 0; tt <= 6; s++, tt++) {
        do { s++; } while (*(s-1) != '\n');
        int ti = 0;
        char* j = s;
        ntags->sizes[tt] = 0;
        for (; *j != '\n'; j++) {
            /* count the amount of tags */
            if (*j != '>' && *(j+1) == '>' && *(j+2) == '>' && *(j+3) != '>') {
                ntags->sizes[tt]++;
            }
        }
        *ptr[tt] = calloc(ntags->sizes[tt], sizeof(char*));

        while (*s != '\n') {
            for (; *s == '>'; s++);
            if (*s != '>' && *s != '\n' && ti % 2 == 0) {
                int i = 0;
                for (; *(s+i) != '>' && *(s+i) != '\n'; i++);
                if (ntags->sizes[tt] > 0) {
                    ptr[tt][0][ti/2] = calloc(i + 1, sizeof(char));
                    strncpy(ptr[tt][0][ti/2], s, i);
                    *(ptr[tt][0][ti/2]+i) = 0;
                }
                s+=i;
            } else {
                for (; *s != '>' && *s != '\n'; s++);
            }
            /* finish */
            if (*s == '>') {
                ti++;
            }
        }
    }
}