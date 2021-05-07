#pragma once

#include <string>
#include <sstream>

#include <cstdarg>

#include <sys/stat.h>

namespace utils {
    inline bool is_dir(const std::string& dir) {
        struct stat st;
        ::stat(dir.c_str(), &st);
        return S_ISDIR(st.st_mode);
    }

    inline bool exist_test(const std::string& name) {
        struct stat buffer;   
        return (::stat(name.c_str(), &buffer) == 0); 
    }

    inline std::string dirname(std::string str) {
        return str.substr(0, str.find_last_of('/'));
    }

    inline bool mkdir_p(const std::string& path) {
        std::string current_level = "";
        std::string level;
        std::stringstream ss(path);

        while (std::getline(ss, level, '/')) {
            current_level += level;
            if (!exist_test(current_level) && ::mkdir(current_level.c_str(), 0755) != 0) {
                return false;
            }
            current_level += "/";
        }
        return true;
    }

    inline std::string strformat(const std::string& fmt, ...) {
        va_list args;
        ::va_start(args, fmt);
        size_t size = ::vsnprintf(nullptr, 0, fmt.c_str(), args) + 1;
        ::va_start(args, fmt);
        char* buf = new char[size];
        ::vsnprintf(buf, size, fmt.c_str(), args);
        ::va_end(args);
        std::string rv(buf, buf + size - 1);
        delete [] buf;
        return rv;
    }
}