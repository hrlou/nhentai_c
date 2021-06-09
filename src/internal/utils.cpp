#include <sstream>
#include <fstream>

#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif

#include <ftw.h>

#include <internal/utils.hpp>

namespace utils {

bool is_dir(const std::string& dir) {
    struct stat st;
    ::stat(dir.c_str(), &st);
    return S_ISDIR(st.st_mode);
}

bool exist_test(const std::string& name) {
    struct stat buffer;   
    return (::stat(name.c_str(), &buffer) == 0); 
}

std::string dirname(std::string str) {
    return str.substr(0, str.find_last_of('/'));
}

int _mkdir(const char* path) {
#ifdef _WIN32
    return ::_mkdir(path);
#else
    return ::mkdir(path, 0755);
#endif
}

bool mkdir_p(const std::string& path) {
    std::string current_level = "";
    std::string level;
    std::stringstream ss(path);

    while (std::getline(ss, level, '/')) {
        current_level += level;
        if (!exist_test(current_level) && _mkdir(current_level.c_str()) != 0) {
            return false;
        }
        current_level += "/";
    }
    return true;
}

std::string read_file(const std::string& name) {
    std::ifstream file(name, std::ios::in);
    std::string src((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return src;
}

}