#pragma once

#include <string>

namespace curl {

std::string download(const std::string& url);
std::vector<std::string> download(const std::vector<std::string>& urls);
void download(const std::string& url, const std::string& file);
void download(const std::vector<std::string>& urls, const std::vector<std::string>& files);

}