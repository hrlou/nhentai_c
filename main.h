#ifndef MAIN_H
#define MAIN_H

char storedData[15000];
int mkdir(const char *pathname, mode_t mode);
int atoi(const char *str);
void progressBar(float num, float den);
void doProcess(char* doujin);
void main(int argc, char **argv);

#endif // MAIN_H