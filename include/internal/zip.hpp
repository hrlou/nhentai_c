#pragma once

#include <string>

void zip_directory(const std::string& inputdir, const std::string& output_filename);
std::string zip_get_file(const std::string& zip_name, const std::string& fname);