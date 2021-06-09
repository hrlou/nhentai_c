#pragma once

#include <string>

namespace utils {

bool is_dir(const std::string& dir);
bool exist_test(const std::string& name);
std::string dirname(std::string str);
int _mkdir(const char* path);
bool mkdir_p(const std::string& path);
std::string read_file(const std::string& name);

}