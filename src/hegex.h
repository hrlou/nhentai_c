// NEED stdio.h stdlib.h

void hegex_search(char searchData[], char **match, char searchAnswer[]) {
    // get size of search data
    int searchBackEnd = 0;
    while (searchAnswer[searchBackEnd] != '\0') {
        searchBackEnd++;
    }

    int searchFrontSize = 0;
    while (searchAnswer[searchFrontSize] != '(') {
        searchFrontSize++;
    }

    int returnIterate = 0;
    int returnStart = 0;

    int charecterIterate = 0;
    while (searchData[charecterIterate] != '\0') {
        // found the charecter, start the search
        if (searchData[charecterIterate] == searchAnswer[0]) {
            int i = 1;
            // check the next charecters as long as the search size
            // stop if we reach the search size or if a charecter is incoreect
            while (searchData[charecterIterate + i] == searchAnswer[i] && i != searchFrontSize) {
                i++;
            }
            // if all the charecters were correct, continue to getting the return
            if (i == searchFrontSize) {
                charecterIterate += i;
                returnStart = charecterIterate;
                returnIterate = charecterIterate;
                // start finding the charecters after the return
                while (searchData[returnIterate] != '\0') {
                    int i;
                    int x = 0;
                    // print the next charecters coresponding to the backsize
                    // searchBackStart would be frontSize + 3
                    if ((searchFrontSize + 3) < searchBackEnd) {
                        for (int i = 0; i < (searchBackEnd - (searchFrontSize + 3) ); i++) {
                            if (searchData[returnIterate + i] == searchAnswer[(searchFrontSize + 3)  + i]) {
                                x++;
                            }
                        }
                        if (x == (searchBackEnd - (searchFrontSize + 3))) {
                            returnIterate--;
                            break;
                        }
                    } else if ((searchFrontSize + 3) == searchBackEnd && searchData[returnIterate] == searchAnswer[searchBackEnd - 1]) {
                        break;
                    }
                    returnIterate++;
                }
                charecterIterate++;
                break;
            }
        }
        charecterIterate++;
    }
    if (returnStart == 0 && returnIterate == 0) {
        printf("\nERROR! No results found\n");
    }
    int i = 0;
    char storeMatches[returnIterate - returnStart + 1];
    *match = malloc((returnIterate - returnStart + 1) * sizeof(char));
    for (int c = returnStart; c < returnIterate; c++) {
        storeMatches[i] = searchData[c];
        i++;
    }
    storeMatches[i] = '\0';
    sprintf(*match, "%s", storeMatches);
}