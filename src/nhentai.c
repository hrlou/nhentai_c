#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "nhentai.h"
#include "ntags.h"
#include "curl.h"
#include "output.h"
#include "search.h"
#include "cbz.h"

nhentai_T* nhentai_init(char* id) {
    nhentai_T* nhentai = calloc(1, sizeof(struct nhentai));
    nhentai->id = id;
    nhentai->url = calloc((25 + strlen(nhentai->id)), sizeof(char));
    snprintf(nhentai->url, (25 + strlen(nhentai->id)), "https://nhentai.net/g/%s/", nhentai->id);
    return nhentai;
}

void nhentai_sanitise(curl_T* mem) {
    size_t i = 0, lines = 0;
    /* count up the lines we want uneffected */
    for (; lines < 5; i++) {
        if (mem->data[i] == '\n') {
            lines++;
        } else if (mem->data[i] == ' ') {
            mem->data[i] = '_';
        }
    }
    size_t j = i;
    /* sanitise the area of the tags */
    for (; i < mem->size && lines < 21; i++, j++) {
        if (mem->data[i] == '\n') {
            lines++;
        } else if (mem->data[i] == ' ') {
            mem->data[i] = '-';
        }
        if (mem->data[i] == '\t') {
            for (; mem->data[i] == '\t'; i++);
        }
        if (mem->data[i] == '<') {
            for (; mem->data[i] != '>'; i++);
        }
        mem->data[j] = mem->data[i];
    }
    mem->size = j;
    mem->data[mem->size] = '\0';
}

void nhentai_html(nhentai_T* nhentai) {
    nhentai->mem = curl_download_html(nhentai->url);
    if (nhentai->mem->size == 0) {
        fputs("failure downloading page; skipping\n", stderr);
        return;
    }
    nhentai_sanitise(nhentai->mem);
}

void nhentai_tags(nhentai_T* nhentai) {
    nhentai->tags = ntags_init(nhentai->mem->data);
    ntags_info(nhentai->tags);
    ntags_tags(nhentai->tags);
}

void nhentai_cbz(nhentai_T* nhentai) {
    char* cbz = calloc((5 + strlen(nhentai->dir)), sizeof(char));
    strcpy(cbz, nhentai->dir);
    strcat(cbz, ".cbz");
    zip_directory(nhentai->dir, cbz);
    return free(cbz);
}

void nhentai_data(nhentai_T* nhentai) {
    nhentai_html(nhentai);
    if (nhentai->mem->size == 0) {
        return;
    }
    nhentai_tags(nhentai);
}

void nhentai_download(nhentai_T* nhentai) {
    if (!nhentai->tags) {
        nhentai_data(nhentai);
    }
    return download_gallery(nhentai);
}

void* nhentai_search(const char* term) {
    search_T* search = search_init(term);
    search_execute(search);
    return search;
}