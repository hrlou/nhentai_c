#pragma once

#include <string>

namespace utils {

bool is_dir(const std::string& path);
bool exist_test(const std::string& name);
std::string dirname(const std::string& path);
std::string read_file(const std::string& name);
bool do_mkdir(const std::string& path);
bool mkpath(std::string path);

}