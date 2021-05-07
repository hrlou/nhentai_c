#pragma once

#include <string>

namespace curl {
    std::string download_html(const std::string& url);
    bool download_file(const std::string& url, const std::string& output);
}