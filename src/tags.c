#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tags.h"

char* sanitise_tags(curl_memory d) {
    char* return_data = (char*)malloc(d.size);  
    int i = 0;
    int newline = 0;
    int writing = 0;
    for (int c = 0; c < d.size; c++) {
        /*  this is dumb, but it makes parsing easier, I plan to redo move to a file buffer approach
            and improve the sanitisation process */
        if (d.data[c] == '\n') {
            newline++;
        }
        if (d.data[c] == '<' || d.data[c] == '\\') {
            writing = 0;
        }
        if (d.data[c] == '>') {
            writing = 1;
        }
        if (newline < 5) {
            writing = 1;
        }
        if (writing == 1) {
            if (d.data[c] == ' ') {
                return_data[i] = '-';
                i++;
            } else if (d.data[c] != '\t') {
                return_data[i] = d.data[c];
                i++;
            } else if (d.data[c] == '&' && d.data[c + 1] == '#' && d.data[c + 2] == '3') {
                return_data[i] = '\'';
                c+=4;
                i++;
            } else if (d.data[c] == '|') {
                return_data[i] = '|';
                i++;
            }
        }
    }
    return_data[i] = '\0';
    free(d.data);
    return return_data;
}

void nhentai_essential(char* s, ntags* tags) {
    for (; *s; s++) {
        if (*s == 'm' && *(s+5) == 'i' && *(s+15) == 'n' && *(s+21) == 'c') {
            // meta-itemprop="name"-content="
            s+=30;
            int i = 0;
            for (; *(s+i+2) != '/'; i++);
            tags->title = (char*)malloc(i+1);
            memcpy(tags->title, s, i);
            // this screws us up sometimes, increasing the allocated size seems to have fixed 
            *(tags->title+i) = 0;
        }
        if (*s == 'm' && *(s+29) == '=' && *(s+53) == 'g' && *(s+61) == 's') {
            // GALLERY ID
            // <meta-itemprop="image"-content="https://t.nhentai.net/galleries/xxxxx/cover.jpg"-/>
            s+=63;
            int i = 0;
            for (; *(s+i) != '/'; i++);
            tags->gallery_id = (char*)malloc(i+1);
            memcpy(tags->gallery_id, s, i);
            *(tags->gallery_id+i) = 0;
        }
        if (*s == 'P' && *(s+6) == '\n') {
            s+=10;
            int i = 0;
            for (; *(s+i) != '>'; i++);
            char* pg;
            pg = (char*)malloc(i+1);
            memcpy(pg, s, i);
            *(pg+i) = 0;
            tags->pages = atoi(pg);
            free(pg);
            break;
        }
    }
}

void nhentai_ident(char* o, ntags* tags) {
    char*** ptr[8] = {&tags->parodies, &tags->characters, &tags->tags, &tags->artists, &tags->groups, &tags->languages, &tags->categories};
    char* s = strstr(o, "Parodies:");
    // >>>xxxx-xxxx>>314>>>>>
    for (int tt = 0; tt <= 6; s++, tt++) {
        do { s++; } while (*(s-1) != '\n');
        int ti = 0;
        char* j = s;
        tags->sizes[tt] = 0;
        for (; *j != '\n'; j++) {
            // count the amount of tags
            if (*j != '>' && *(j+1) == '>' && *(j+2) == '>' && *(j+3) != '>') {
                tags->sizes[tt]++;
            }
        }
        *ptr[tt] = (char **)calloc(tags->sizes[tt], sizeof(char*));

        while (*s != '\n') {
            for (; *s == '>'; s++);
            if (*s != '>' && *s != '\n' && ti % 2 == 0) {
                int i = 0;
                for (; *(s+i) != '>' && *(s+i) != '\n'; i++);
                if (tags->sizes[tt] > 0) {
                    ptr[tt][0][ti/2] = (char*)malloc(i);
                    memcpy(ptr[tt][0][ti/2], s, i);
                    *(ptr[tt][0][ti/2]+i) = 0;
                }
                s+=i;
            } else {
                for (; *s != '>' && *s != '\n'; s++);
            }
            // finish
            if (*s == '>') {
                ti++;
            }
        }
    }

}

ntags nhentai_tags(char* id) {
    ntags h;
    h.id = id;
    char *current_url = (char *) malloc(36);
    snprintf(current_url, 36, "https://nhentai.net/g/%s/", id);
    char* s = sanitise_tags(get_html(current_url));
    nhentai_essential(s, &h);
    if (h.gallery_id == NULL) {
        fprintf(stderr, "\033[0;31mInvalid ID: \033[1m%s\033[0m\n", h.id);
    } else {
        nhentai_ident(s, &h);
    }
    free(s);
    return h;
}