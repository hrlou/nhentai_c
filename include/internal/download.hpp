#pragma once

#include <vector>
#include <string>

#include <sys/stat.h>

void download_array(const std::string* urls, const std::string* files, size_t num);
inline void download_vector(std::vector<std::string> urls, std::vector<std::string> files) {
    return download_array(&urls[0], &files[0], (urls.size() < files.size()) ? urls.size() : files.size());
}