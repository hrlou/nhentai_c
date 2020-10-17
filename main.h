#ifndef MAIN_H
#define MAIN_H

#define LIMIT 75

char storedData[15000];
char tagsData[100];
int mkdir(const char *pathname, mode_t mode);
int atoi(const char *str);
void progressBar(float num, float den);
void getTags(char* id, char* name);
int main(int argc, char **argv);

#endif // MAIN_H