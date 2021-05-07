#pragma once

#include <vector>
#include <string>

#include <sys/stat.h>

void progress_bar(std::ostream& out, float numerator, float denominator);
void array_download(const std::string* urls, const std::string* files, size_t num);

inline void vector_download(std::vector<std::string> urls, std::vector<std::string> files) {
    return array_download(&urls[0], &files[0], (urls.size() < files.size()) ? urls.size() : files.size());
}