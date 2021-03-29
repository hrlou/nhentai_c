#ifndef OUTPUT_H
#define OUTPUT_H

#include "ntags.h"
#include "nhentai.h"

int remove_recursive(char* path);
void directory_gen(nhentai_T* nhentai);
void download_gallery(nhentai_T* nhentai);

#endif /* OUTPUT_H */