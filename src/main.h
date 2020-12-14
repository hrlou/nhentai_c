#ifndef MAIN_H
#define MAIN_H

#include "tags.h"
#include "../config.def.h"

#ifndef __FreeBSD__
int mkdir(const char *pathname, int mode);
#endif

void progress_bar(float num, float den);
void nhentai_download(ntags nhentai);
int main(int argc, char **argv);

#endif // MAIN_H
