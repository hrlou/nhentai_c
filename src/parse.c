#include <stdio.h>
#include <stdlib.h>

#ifdef __FreeBSD__
#include <sys/stat.h>
#endif

#include "main.h"
#include "download.h"
#include "parse.h"
#include "../config.def.h"

char* get_range(char data[], int start, int end) {
    int count = 0;
    char* return_value = (char*) malloc((end - start));
    for (int i = start; i < end; i++) {
        return_value[count] = data[i];
        count++;
    }
    return return_value;
}

char* sanitise_tags(char* buf, size_t size) {
    char* return_data = (char*)malloc(size);  
    int i = 0;
    int newline = 0;
    int writing = 0;
    for (int c = 0; c < size; c++) {
        // this is horrible, but it saves memory and makes parsing easier
        if (buf[c] == '\n') {
            newline++;
        }
        if (buf[c] == '<' || buf[c] == '\\') {
            writing = 0;
        }
        if (buf[c] == '>') {
            writing = 1;
        }
        if (newline < 5) {
            writing = 1;
        }
        if (writing == 1) {
            if (buf[c] == ' ') {
                return_data[i] = '-';
                i++;
            } else if (buf[c] != '\t') {
                return_data[i] = buf[c];
                i++;
            } else if (buf[c] == '&' && buf[c + 1] == '#' && buf[c + 2] == '3') {
                return_data[i] = '\'';
                c+=4;
                i++;
            }
        }
    }
    return return_data;
}

ntags parse_tags(curl_memory data) {
    ntags tags;
    char* stored_data = sanitise_tags(data.data, data.size);
    /*printf("%s\n", stored_data);
    char tags_stored[10][25][100];
    char *tag_types[] = {"Title", "Gallery-Id", "Parodies", "Characters", "Tags", "Artists", "Groups", "Languages", "Categories", "Pages"};
    int tag_sizes[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};*/
    int tag_sizes[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    char** tags_point[10];
    tags_point[0] = &tags.title;
    tags_point[1] = &tags.gallery_id;
    tags_point[2] = &tags.parodies;
    tags_point[3] = &tags.characters;
    tags_point[4] = &tags.tags;
    tags_point[5] = &tags.artists;
    tags_point[6] = &tags.groups;
    tags_point[7] = &tags.languages;
    tags_point[8] = &tags.categories;
    tags_point[9] = &tags.pages;


    int c = 0;
    int tag_type = 0;
    // puts(stored_data);
    while (stored_data[c] != '\0') {
        if (stored_data[c] == 'm' && stored_data[c + 5] == 'i' && stored_data[c + 15] == 'n' && stored_data[c + 21] == 'c') {
            // TITLE
            c += 30;
            int i = c;
            while (stored_data[i + 2] != '/') {
                i++;
            }
            *tags_point[tag_type] = get_range(stored_data, c, i);
            tag_type++;
            c = i;
        } else if (stored_data[c] == 'm' && stored_data[c + 29] == '=' && stored_data[c + 53] == 'g' && stored_data[c + 61] == 's') {
            // GALLERY ID
            // <meta-itemprop="image"-content="https://t.nhentai.net/galleries/941176/cover.jpg"-/>
            c+=63;
            int i = c;
            while (stored_data[i] != '/') {
                i++;
            }
            *tags_point[tag_type] = get_range(stored_data, c, i);
            tag_sizes[tag_type] = 1;
            tag_type++;
        }/* else if (stored_data[c] == 'P' && stored_data[c + 2] == 'r' && stored_data[c + 8] == ':') {
            // Parodies:
            // >>>princess-connect>>314>>>>>
            int line_count = 0;
            while (line_count < 15) {
                do { c++; } while (stored_data[c - 1] != '\n');
                line_count++;

                int tag_store_count = 0;
                int test_iterate = 0;
                while (stored_data[c] != '\n') {
                    // get past the >
                    while (stored_data[c] == '>') {
                        c++;
                    }
                    // if we are saving
                    if (stored_data[c] != '>' && stored_data[c] != '\n' && test_iterate % 2 == 0) {
                        int i = 0;
                        while (stored_data[c] != '>' && stored_data[c] != '\n') {
                            // putchar(stored_data[c]);
                            tags_stored[tag_type][tag_store_count][i] = stored_data[c];
                            c++;
                            i++;
                        }
                        tags_stored[tag_type][tag_store_count][i] = '\0';
                        tag_sizes[tag_type] = (tag_store_count + 1);
                        // printf("tags_stored %d %d    %s\n", tag_type, tag_sizes[tag_type], tags_stored[tag_type][tag_store_count]);
                        tag_store_count++;
                    } else {
                        // skip
                        while (stored_data[c] != '>' && stored_data[c] != '\n') {
                            c++;
                        }
                    }
                    // finish
                    if (stored_data[c] == '>') {
                        test_iterate++;
                    }
                }
                c++;
                line_count++;
                tag_type++;
            }
        }*/
        c++;
    }
    // make the directory name
    // this was erroring because sometimes size is becoming too big, I'm unsure if this is still a problem since the bufsize is so large
    /*size_t bufsize = 128;
    char *directory = (char *) malloc(bufsize);
    snprintf(directory, bufsize, "%s", doujin_id);
    if (NAMING != 0) {
        size_t size = snprintf(directory, bufsize, "%s_", doujin_id);
        // size++;
        for (int x = 0; x < tag_sizes[(NAMING - 1)]; x++) {
            // puts(tags_stored[(NAMING - 1)][x]);
            // printf("%ld\n", size);
            if ((x + 1) >= tag_sizes[(NAMING - 1)]) {
                size += snprintf(directory+size, bufsize, "%s", tags_stored[(NAMING - 1)][x]);
            } else {
                size += snprintf(directory+size, bufsize, "%s_", tags_stored[(NAMING - 1)][x]);
            }
            
            if (size >= bufsize) {
                bufsize += 64;
                directory = (char *) realloc(directory, bufsize);
            }
        }
    }

    mkdir(directory, 0777);

    char *tags_file = (char*) malloc(bufsize + 15);
    snprintf(tags_file, (bufsize + 15), "%s/%s.txt", directory, doujin_id);

    // write tags to file
    FILE *file_write;
    file_write = fopen(tags_file, "w");
    free(tags_file);
    for (int y = 0; y < 10; y++) {
        fprintf(file_write, "%s: ", tag_types[y]);
        for (int x = 0; x < tag_sizes[y]; x++) {
            if ((x + 1) >= tag_sizes[y]) {
                fprintf(file_write, "%s", tags_stored[y][x]);
            } else {
                fprintf(file_write, "%s, ", tags_stored[y][x]);
            }
        }
        fprintf(file_write, "\n");
    }
    fclose(file_write);
    for (int y = 0; y < 10; y++) {
        fprintf(stderr, "%s: ", tag_types[y]);
        for (int x = 0; x < tag_sizes[y]; x++) {
            if ((x + 1) >= tag_sizes[y]) {
                fprintf(stderr, "%s", tags_stored[y][x]);
            } else {
                fprintf(stderr, "%s, ", tags_stored[y][x]);
            }
        }
        fprintf(stderr, "\n");
    }*/
    return tags;
}