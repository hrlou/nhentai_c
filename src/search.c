#include <stdio.h>
#include <stdlib.h>

#include "search.h"
#include "main.h"
#include "download.h"
#include "parse.h"

#define SEARCH_LIMIT 100
#define SCREEN_LIMIT 10
#define LIMIT 25

void do_search(char *search_terms, int size) {
	// "https://nhentai.net/search/?q=""${search}""&sort=popular&page=${page}" 50
	char** searched_ids;
    int page_current = 1;
    char* current_url;
	do {
		char* current_url = (char*) malloc(size + 52);
		snprintf(current_url, (size + 52), "https://nhentai.net/search/?q=%s&sort=popular&page=%d", search_terms, page_current);
        puts(current_url);
		get_html(current_url, 1);
        free(current_url);
		page_current++;
		searched_ids = parse_search();
		// printf("%s\n", searched_ids[0]);
		for (int i = 1; i < atoi(searched_ids[0]); i+=2) {
			printf("%s\t%s\n", searched_ids[i], searched_ids[i + 1]);
		}
        printf("\n%d\n\n", (atoi(searched_ids[0]) / 2));
		// exit(0);
        int i = 0;
        while (stored_data[i] != '\0') {
            stored_data[i] = '\0';
            i++;
        }
	} while((atoi(searched_ids[0]) / 2) > 0);
}

int search_menu(void) {
	/*	currently a demo of how this feature will work
		these values are preset */
	printf("Input Search Term:\n");
   	char search_terms[SEARCH_LIMIT];
   	fgets(search_terms, SEARCH_LIMIT, stdin);
   	int i = 0;
   	while (search_terms[i] != '\n') {
   		if (search_terms[i] == ' ') {
   			search_terms[i] = '+';
   		}
   		i++;
   	}
   	search_terms[i] = '\0';
   	do_search(search_terms, i);
}
