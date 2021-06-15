#pragma once

#include <string>

namespace curl {
    std::string download_page(const std::string& url);
    bool download_file(const std::string& url, const std::string& _output);
}