#ifndef MAIN_H
#define MAIN_H

#ifndef __FreeBSD__
int mkdir(const char *pathname, int mode);
#endif

void progress_bar(float num, float den);
void nhentai_download(char* doujin);
int main(int argc, char **argv);

#endif // MAIN_H
