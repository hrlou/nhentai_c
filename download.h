int doDownload(char* url, char* file) {
    CURL* image;
    CURLcode imgresult;
    FILE *fp;

    // printf("%s\n", url);
    // if (!existTest() {
        image = curl_easy_init();
        fp = fopen(file, "w");
        if (fp == NULL) {
            printf("Unable To Create Image\n");
        }
        curl_easy_setopt(image, CURLOPT_URL, url);
        curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(image, CURLOPT_FAILONERROR, true);
        curl_easy_setopt(image, CURLOPT_WRITEDATA, fp);
        
        imgresult = curl_easy_perform(image);

        if (imgresult) {
            curl_easy_cleanup(image);
            fclose(fp);
            printf("ERROR! on downloading pg.\n");
            return 0;
        }
        curl_easy_cleanup(image);
        fclose(fp);

    // }
    return 1; 
}