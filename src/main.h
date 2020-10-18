#ifndef MAIN_H
#define MAIN_H

#define LIMIT 75

char storedData[15000];
int mkdir(const char *pathname, int mode);
int atoi(const char *str);
void progressBar(float num, float den);
void getTags(char* id, char* name, char* gid, char* pages);
int main(int argc, char **argv);

#endif // MAIN_H