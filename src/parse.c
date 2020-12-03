#include <stdio.h>
#include <stdlib.h>

#ifdef __FreeBSD__
#include <sys/stat.h>
#endif

#include "main.h"
#include "../config.def.h"

// needs to be defined
// char stored_data[15000];
char stored_data[30000];
// char *stored_data;

char* get_range(char data[], int start, int end) {
    int count = 0;
    char* return_value = (char*) malloc((end - start));
    for (int i = start; i < end; i++) {
        return_value[count] = data[i];
        count++;
    }
    return return_value;
}

char** parse_search(void) {
    int c = 0;
    int count = 1;
    char **searched_ids = (char**) malloc(51 * sizeof(char*));
    while (stored_data[c] != '\0') {
        if (stored_data[c] == '<' && stored_data[c + 10] == 'g' && stored_data[c + 11] != 'r') {
            c+=12;
            int start = c;
            while (stored_data[c] != '/') {
                c++;
            }
            searched_ids[count] = get_range(stored_data, start, c);
            count++;
            while (1) {
                c++;
                if (stored_data[c] == '"' && stored_data[c + 1] == 'c' && stored_data[c + 2] == 'a' && stored_data[c + 3] == 'p' && stored_data[c + 4] == 't') {
                    c+=10;
                    // shorten the names
                    if (stored_data[c] == '(') {
                        while (stored_data[c - 1] != ')') {
                            c++;
                        }
                        if (stored_data[c] == ' ') {
                            c++;
                        }
                    }
                    if (stored_data[c] == '[') {
                        while (stored_data[c - 1] != ']') {
                            c++;
                        }
                        if (stored_data[c] == ' ') {
                            c++;
                        }
                    }
                    int start = c;
                    while (stored_data[c + 1] != '(' && stored_data[c] != '<') {
                        if (stored_data[c] != '\n') {
                            c++;
                        }
                    }
                    searched_ids[count] = get_range(stored_data, start, c);
                    break;
                }
            }
            count++;
        }
        c++;
    }
    sprintf(searched_ids[0], "%d", (count - 1));
    return searched_ids;
}

char *parse_tags(char* doujin_id, char* gallery_id, char* page_count) {
    char tags_stored[10][25][100];
    char *tag_types[] = {"Title", "Gallery-Id", "Parodies", "Characters", "Tags", "Artists", "Groups", "Languages", "Categories", "Pages"};
    int tag_sizes[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    int c = 0;
    int tag_type = 0;
    // puts(stored_data);
    while (stored_data[c] != '\0') {
        if (stored_data[c] == 'm' && stored_data[c + 5] == 'i' && stored_data[c + 15] == 'n' && stored_data[c + 21] == 'c') {
            // TITLE
            c += 30;
            int i = 0;
            while (stored_data[c + 2] != '/') {
                tags_stored[tag_type][0][i] = stored_data[c];
                c++;
                i++;
            }
            tags_stored[tag_type][0][i] = '\0';
            puts(tags_stored[tag_type][0]);
            tag_sizes[tag_type] = 1;
            tag_type++;
        } else if (stored_data[c] == 'm' && stored_data[c + 29] == '=' && stored_data[c + 53] == 'g' && stored_data[c + 61] == 's') {
            // GALLERY ID
            // <meta-itemprop="image"-content="https://t.nhentai.net/galleries/941176/cover.jpg"-/>
            c+=63;
            int i = 0;
            while (stored_data[c] != '/') {
                tags_stored[tag_type][0][i] = stored_data[c];
                c++;
                i++;
            }
            tags_stored[tag_type][0][i] = '\0';
            tag_sizes[tag_type] = 1;
            tag_type++;
        } else if (stored_data[c] == 'P' && stored_data[c + 2] == 'r' && stored_data[c + 8] == ':') {
            /*Parodies:
            >>>princess-connect>>314>>>>>*/
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
        }
        c++;
    }
    // make the directory name
    // this was erroring because sometimes size is becoming too big, I'm unsure if this is still a problem since the bufsize is so large
    size_t bufsize = 128;
    char *directory = (char *) malloc(bufsize);
    snprintf(directory, bufsize, "%s", doujin_id);
    /* Generate a name by going through the tag type specified in config.def.h */
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

    /*for (int y = 0; y < 10; y++) {
        printf("%s: ", tag_types[y]);
        for (int x = 0; x < tag_sizes[y]; x++) {
            if ((x + 1) >= tag_sizes[y]) {
                printf("%s", tags_stored[y][x]);
            } else {
                printf("%s, ", tags_stored[y][x]);
            }
        }
        printf("\n");
    }*/

    snprintf(gallery_id, 10, "%s", tags_stored[1][0]);
    snprintf(page_count, 5, "%s", tags_stored[9][0]);
    return directory;
}