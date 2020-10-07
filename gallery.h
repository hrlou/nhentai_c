void searchGalleryInfo(char **id, char **ex) {
    int searchIterate = 0;
    int searchStrSize = 32;
    int searchAnswer[] = { 'h', 't', 't', 'p', 's', ':', '/', '/', 'i', '.', 'n', 'h', 'e', 'n', 't', 'a', 'i', '.', 'n', 'e', 't', '/', 'g', 'a', 'l', 'l', 'e', 'r', 'i', 'e', 's', '/'};
    bool searchStart = false;
    bool foundStart = false;
    bool getId = false;
    bool getEx = false;

    int codeStart = 0;
    int codeEnd = 0;

    int exStart = 0;
    int exEnd = 0;
    // https://i.nhentai.net/galleries/1163557/1.jpg" 

    int i = 0;
    while (i < writeDataI) {
	    if (searchIterate == searchStrSize) {
	        // do when string is found
	        searchIterate = 0;
	        searchStart = false;
	        foundStart = true;
	    }

	    if (foundStart == true) {
	    	if (getEx == false) {
	    		// start getting id
	    		getId = true;
	    	}

	    	if (getId == true && storedData[i] == '/') {
	    		// start getting extention
	    		getId = false;
	    		getEx = true;
	    		codeEnd = i;
	    		i++;
	    	} else if (getId == true) {
	    		// set the begining for the index
	    		if (codeStart == 0) { 
	    			codeStart = i;
	    		}
	    	}

	    	if (getEx == true && storedData[i] == '"') {
	    		// stop all search
	    		getEx == false;
	    		foundStart = false;
	    		exEnd = i;
	    	} else if (getEx == true) {
	    		// set the begining for the index
	    		if (exStart == 0) { 
	    			exStart = i;
	    		}
	    	}
	    }

	    // find out to continue search
	    if (searchStart == true) {
	        // continue
	        if (storedData[i] == searchAnswer[searchIterate]) {
	            searchIterate++;
	        // stop search
	        } else {
	            searchIterate = 0;
	            searchStart = false;
	        }

	    }

	    // start the search if charecter is found
	    if (storedData[i] == searchAnswer[searchIterate] && searchStart == false) {
	        searchStart = true;
	        searchIterate++;
	    }
		i++;
	}
	// char galleryId[codeEnd - codeStart];
	char arrId[codeEnd - codeStart];
	char arrEx[exEnd - exStart];
	arrId[codeEnd - codeStart] = '\0';
	arrEx[exEnd - exStart] = '\0';

	// initialise pointers
	char* galleryId = malloc((codeEnd - codeStart) * sizeof(char));
	char* galleryEx = malloc((exEnd - exStart) * sizeof(char));
	
	// set value of arrays
	int x = 0;
	for (short i = codeStart; i < codeEnd; i++) {
		arrId[x] = storedData[i];
		x++;
	}
	x = 0;
	for (short i = exStart; i < exEnd; i++) {
		arrEx[x] = storedData[i];
		x++;
	}

	// convert arrays to char pointers
	sprintf(galleryId, "%s", arrId);
	sprintf(galleryEx, "%s", arrEx);

	// return values
	*id = galleryId;
	*ex = galleryEx;
}

int getGalleryId(char* downloadUrl) {
    CURL* getId; // our curl object
    CURLcode getIdResult;
    getId = curl_easy_init(); // easy int
    curl_easy_setopt(getId, CURLOPT_URL, downloadUrl); // make url the curl_url
    curl_easy_setopt(getId, CURLOPT_WRITEFUNCTION, &writeCallBack);
    getIdResult = curl_easy_perform(getId); // perform operation
   
    if (getIdResult) {
        curl_easy_cleanup(getId);
        return 0;
    }
    return 1;
}