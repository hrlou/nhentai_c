#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tags.h"

char* get_range(char data[], int start, int end) {
    int count = 0;
    int i = start;
    char* return_value = (char*) malloc(((end + 1) - start));
    for (; i < end; i++) {
        return_value[count] = data[i];
        count++;
    }
    return_value[count] = '\0';

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
    // printf("%s\n", stored_data);
    char*** tags_point[8] = {&tags.parodies, &tags.characters, &tags.tags, &tags.artists, &tags.groups, &tags.languages, &tags.categories};

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
            tags.title = get_range(stored_data, c, i);
            c = i;
        } else if (stored_data[c] == 'm' && stored_data[c + 29] == '=' && stored_data[c + 53] == 'g' && stored_data[c + 61] == 's') {
            // GALLERY ID
            // <meta-itemprop="image"-content="https://t.nhentai.net/galleries/xxxxx/cover.jpg"-/>
            c+=63;
            int i = c;
            while (stored_data[i] != '/') {
                i++;
            }
            tags.gallery_id = get_range(stored_data, c, i);
            c = i;
        } else if (stored_data[c] == 'P' && stored_data[c + 2] == 'r' && stored_data[c + 8] == ':') {
            // Parodies:
            // >>>xxxx-xxxx>>314>>>>>
            int line_count = 0;
            while (line_count < 15) {
                do { c++; } while (stored_data[c - 1] != '\n');
                line_count++;

                int tag_store_count = 0;
                int test_iterate = 0;

                if (tag_type != 7) {
                    int j = c;
                    tags.sizes[tag_type] = 0;
                    while (stored_data[j] != '\n') {
                        if ((stored_data[j] >= 'a' && stored_data[j] <= 'z' && stored_data[j] != 'K') || stored_data[j] == '-') {
                            tags.sizes[tag_type]++;
                            while ((stored_data[j] >= 'a' && stored_data[j] <= 'z' && stored_data[j] != 'K') || stored_data[j] == '-') {
                                j++;
                            }
                        }
                        j++;
                    }
                    *tags_point[tag_type] = (char **)calloc(tags.sizes[tag_type] + 1, sizeof(char*));
                }

                while (stored_data[c] != '\n') {
                    // get past the >
                    while (stored_data[c] == '>') {
                        c++;
                    }
                    if (stored_data[c] != '>' && stored_data[c] != '\n' && test_iterate % 2 == 0) {
                        // 1 2 3 1 1 1 1 1
                        int i = c;
                        if (tag_type == 7) {
                            while (stored_data[i] != '>' && stored_data[i] != '\n') {
                                i++;
                            }
                            tags.pages = atoi(get_range(stored_data, c, i));
                            c = i;
                        } else {
                            while (stored_data[i] != '>' && stored_data[i] != '\n') {
                                i++;
                            }
                            tags_point[tag_type][0][tag_store_count] = get_range(stored_data, c, i);
                            c = i;
                            tag_store_count++;
                        }
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
    return tags;
}

ntags nhentai_tags(char* id) {
    char *current_url = (char *) malloc(35);
    snprintf(current_url, 35, "https://nhentai.net/g/%s/", id);
    ntags nhentai = parse_tags(get_html(current_url));
    nhentai.id = id;
    return nhentai;
}