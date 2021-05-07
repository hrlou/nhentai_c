#pragma once

#include "nhentai.hpp"
#include "config.h"

#include <cstdio>

#include <vector>
#include <string>

void progress_bar(FILE* fd, float numerator, float denominator);
void array_download(const std::string* urls, const std::string* files, size_t num);
void array_archive(const std::string* files, const std::string out, size_t num);

inline void vector_download(std::vector<std::string> urls, std::vector<std::string> files) {
    return array_download(&urls[0], &files[0], (urls.size() < files.size()) ? urls.size() : files.size());
}

inline std::string dirname(std::string str) {
    return str.substr(0, str.find_last_of('/'));
}

inline bool exist_test(const std::string name) {
    FILE *file;
    if ((file = fopen(name.c_str(), "r")) != NULL) {
        fclose(file);
        return true;
    }
    return false;
}