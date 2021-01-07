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
    return return_data;
}

ntags nhentai_tags(char* id) {
    ntags tags;
    char *current_url = (char *) malloc(36);
    snprintf(current_url, 36, "https://nhentai.net/g/%s/", id);
    // current_url = "https://nhentai.net/g/";
    tags.id = id;
    char* s = sanitise_tags(get_html(current_url));
    char*** ptr[8] = {&tags.parodies, &tags.characters, &tags.tags, &tags.artists, &tags.groups, &tags.languages, &tags.categories};
    for (; *s; s++) {
        // this just speeds up the process, not having to always do the if statements
        for (; *s != 'P' && *s != 'm'; s++);
        if (*s == 'm' && *(s+5) == 'i' && *(s+15) == 'n' && *(s+21) == 'c') {
            s+=30;
            int i = 0;
            for (; *(s+i+2) != '/'; i++);
            tags.title = (char*)malloc(i);
            memcpy(tags.title, s, i);
            *(tags.title+i) = 0;
        } else if (*s == 'm' && *(s+29) == '=' && *(s+53) == 'g' && *(s+61) == 's') {
            // GALLERY ID
            // <meta-itemprop="image"-content="https://t.nhentai.net/galleries/xxxxx/cover.jpg"-/>
            s+=63;
            int i = 0;
            for (; *(s+i) != '/'; i++);
            tags.gallery_id = (char*)malloc(i);
            memcpy(tags.gallery_id, s, i);
            *(tags.gallery_id+i) = 0;
        } else if (*s == 'P' && *(s+2) == 'r' && *(s+8) == ':') {
            // Parodies:
            // >>>xxxx-xxxx>>314>>>>>
            for (int tt = 0; tt < 8; s++, tt++) {
                do { s++; } while (*(s-1) != '\n');
                int ti = 0;

                if (tt != 7) {
                    char* j = s;
                    tags.sizes[tt] = 0;
                    for (; *j != '\n'; j++) {
                        // count the amount of tags
                        if (*j != '>' && *(j+1) == '>' && *(j+2) == '>' && *(j+3) != '>') {
                            tags.sizes[tt]++;
                        }
                    }
                    // printf("%d\n", tags.sizes[tt]);
                    if (tags.sizes[tt] > 0) {
                        *ptr[tt] = (char **)calloc(tags.sizes[tt], sizeof(char*));
                    }
                }

                while (*s != '\n') {
                    for (; *s == '>'; s++);
                    if (*s != '>' && *s != '\n' && ti % 2 == 0) {
                        // char* j = s;
                        int i = 0;
                        for (; *(s+i) != '>' && *(s+i) != '\n'; i++);
                        if (tt == 7) {
                            tags.pages = 10;
                        }
                        // I THINK THE ERRORS ARE CAUSED BY NULL TAGS
                        /*if (tt == 7) {
                            char* pg;
                            pg = (char*)malloc(i);
                            memcpy(pg, s, i);
                            *(pg+i) = 0;
                            tags.pages = atoi(pg);
                            free(pg);
                        } else if (tags.sizes[tt] > 0) {
                            ptr[tt][0][ti/2] = (char*)malloc(i);
                            memcpy(ptr[tt][0][ti/2], s, i);
                            *(ptr[tt][0][ti/2]+i) = 0;
                            // puts(ptr[tt][0][ti/2]);
                        }*/
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
    }
    puts("Return");
    return tags;
}