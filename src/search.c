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
	char* current_url = (char*) malloc(size + 52);
	int page_current = 1;
	do {
		int c = 0;
		while (stored_data[c] != '\0') {
			stored_data[c] = ' ';
			c++;
		}
		snprintf(current_url, (size + 52), "https://nhentai.net/search/?q=%s&sort=popular&page=%d", search_terms, page_current);
		get_html(current_url, 1);
		puts(current_url);
		page_current++;
	} while (parse_search() != 0);
	exit(0);
	// puts(current_url);
	// current_url[(size + 49)] = '2';
	// puts(current_url);
	
}

int search_menu(void) {
	/*	currently a demo of how this feature will work\
		these values are preset */
	/*printf("Input Search Term:\n");
   	char search_terms[SEARCH_LIMIT];
   	fgets(search_terms, SEARCH_LIMIT, stdin);
   	int i = 0;
   	while (search_terms[i] != '\n') {
   		if (search_terms[i] == ' ') {
   			search_terms[i] = '+';
   		}
   		i++;
   	}
   	search_terms[i] = '\0';*/
   	// do_search(search_terms, i);
   	do_search("shiki-ichinose+japanese", 23);
	char *search_ids[LIMIT] = {"253566", "305586", "153769", "254048", "164586", "172788", "278421", "257136", "256121", "219249", "306902", "319329", "207372", "179001", "191772", "179871", "190100", "232557", "254000", "191598", "219165", "174828", "289789", "290911", "258819"};
	
	int starting_point = 1;
	for (int i = starting_point; i <= SCREEN_LIMIT; ++i) {
		printf("%d:\t%s\n", i, search_ids[(i - 1)]);
	}
	// printf("exit (0), select doujin (number), next page (%d): ", (SCREEN_LIMIT + 1));
	char doujin_choice = 'z';
	while (doujin_choice == 'z') {
		printf("download all (a), dump to text (d), select a doujin (s): ");
		scanf("%c", &doujin_choice);
		getchar();
		switch (doujin_choice) {
			case 'a':
				for (int i = 0; i < LIMIT; i++) {
					printf("Downloading (%d/%d) %s :", (i + 1), LIMIT, search_ids[i]);
					doujin_download(search_ids[i]);
					putchar('\n');
				}
				break;
			case 'd':
				for (int i = 0; i < LIMIT; i++) {
					printf("%s ", search_ids[i]);
				}
				break;
			case 's':
				break;
			default:
				doujin_choice = 'z';
				break;
		}
 			

		/*
			printf("Selected (%s)\n", search_ids[(doujin_choice - 1)]);
			// download
			printf("download (d), tags (t), preview (p), back (b), exit (e): ");
			char user_input;
			scanf("%c", &user_input);
			getchar();
			if (user_input == 'd') {
				printf("Downloading (1/1) %s : ", search_ids[(doujin_choice - 1)]);
				doujin_download(search_ids[(doujin_choice - 1)]);
				putchar('\n');
			}
			*/
	}
}