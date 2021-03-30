#pragma once

#include "ntags.h"
#include "nhentai.h"

int remove_recursive(char* path);
void directory_gen(const char* fmt, nhentai_T* nhentai);
void download_gallery(nhentai_T* nhentai);