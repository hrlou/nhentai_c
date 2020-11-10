#ifndef MAIN_H
#define MAIN_H

char storedData[15000];
// gcc-10.2
// extern char storedData[15000];
int mkdir(const char *pathname, int mode);
int atoi(const char *str);
void progressBar(float num, float den);
char *getTags(char* id, char* galleryId, char* pages);
int main(int argc, char **argv);

#endif // MAIN_H