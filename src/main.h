#ifndef MAIN_H
#define MAIN_H

// extern char stored_data[15000];
extern char stored_data[30000];

#ifndef __FreeBSD__
int mkdir(const char *pathname, int mode);
#endif

void progress_bar(float num, float den);
void doujin_download(char* doujin);
int main(int argc, char **argv);

#endif // MAIN_H
