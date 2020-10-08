// NEED stdio.h, stdlib.h
// typedef enum {false, true} bool;

void hegex_search(char searchData[], char **match, char searchAnswer[]) {
    int searchIterate = 0;
    int searchFullSize = 0;
    // get size of search data
    while (searchAnswer[searchFullSize] != '\0') {
        searchFullSize++;
    }
    int sizeOfData = 0;
    while (searchData[sizeOfData] != '\0') {
        sizeOfData++;
    }

    int searchDefineIterate = 0;
    int searchFrontSize = 0;
    while (searchAnswer[searchDefineIterate] != '(' && searchAnswer[searchDefineIterate - 1] != '\\') {
        searchFrontSize++;
        searchDefineIterate++;
    }
    searchDefineIterate+=3;
    int searchBackStart = searchDefineIterate;
    int searchBackEnd = searchFullSize;
    int searchBackSize = (searchBackEnd - searchBackStart);

    // printf("%d\n%d\n", searchBackStart, searchBackEnd);
    
    int charecterIterate = 0;
    int returnIterate = 0;
    int returnStart = 0;
    int testIterate = 0;

    bool startTest = false;
    bool startReturn = false;

    for (charecterIterate = 0; charecterIterate < sizeOfData; charecterIterate++) {
        // putchar(searchData[charecterIterate]);

        // begin getting coordinates to the return
        if (startReturn == true) {
            returnStart = charecterIterate;
            returnIterate = charecterIterate;
            // go through remaining charecters
            while (returnIterate < sizeOfData) {
                int i;
                int x = 0;
                testIterate = charecterIterate;
                // print the next charecters coresponding to the backsize
                if (searchBackStart < searchBackEnd) {
                    for (int i = 0; i < searchBackSize; i++) {
                        // if backdata is same as what's behind, add to x
                        if (searchData[returnIterate + i] == searchAnswer[searchBackStart + i]) {
                            x++;
                        }
                    }
                    // quit if x is BackSize
                    if (x == searchBackSize) {
                        startReturn = false;
                        returnIterate--;
                        break;
                    }
                // if backdata is only one charecter long
                } else if (searchBackStart == searchBackEnd) {
                    if (searchData[returnIterate] == searchAnswer[searchBackEnd - 1]) {
                        startReturn = false;
                        break;
                    }
                }
                returnIterate++;
            }
            break;
        // if backdata is NULL
        } else {
            if (startTest == true) {
                if (searchData[charecterIterate] == searchAnswer[searchIterate]) {
                    // putchar('?');
                    searchIterate++;
                } else {
                    searchIterate = 0;
                    startTest = false;
                }
            }

            // if we find the charecter, start the test
            if (searchData[charecterIterate] == searchAnswer[searchIterate] && startTest == false) {
                startTest = true;
                searchIterate++;
            }

            // start the return when the front is finished
            if (searchIterate == searchFrontSize) {
                startReturn = true;
            }
        }
    }
    // error if no result
    if (returnStart == 0 && returnIterate == 0) {
        printf("Nothing Found\n");
    }

    int i = 0;
    char storeMatches[returnIterate - returnStart + 1];
    *match; //= malloc((returnIterate - returnStart + 1) * sizeof(char));
    for (int c = returnStart; c < returnIterate; c++) {
        storeMatches[i] = searchData[c];
        i++;
    }
    storeMatches[i] = '\0';
    sprintf(*match, "%s", storeMatches);
}