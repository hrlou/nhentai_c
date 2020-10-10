#ifndef DOWNLOAD_H
#define DOWNLOAD_H

char storedData[15000];
static size_t writeCallBack(char* buf, size_t size, size_t nmemb, void* up);
int getHtml(const char* downloadUrl);
int doDownload(char* url, char* file);

#endif // DOWNLOAD_H