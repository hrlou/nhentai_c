#include <sstream>
#include <fstream>

#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define MKDIR(PATH) ::_mkdir(PATH)
#else
#define MKDIR(PATH) ::mkdir(PATH, 0755)
#endif

#include <internal/utils.hpp>

namespace utils {

bool is_dir(const std::string& path) {
    struct stat st;
    ::stat(path.c_str(), &st);
    return S_ISDIR(st.st_mode);
}

bool exist_test(const std::string& name) {
    struct stat st;   
    return (::stat(name.c_str(), &st) == 0); 
}

std::string dirname(const std::string& path) {
    return path.substr(0, path.find_last_of('/'));
}

std::string read_file(const std::string& name) {
    std::ifstream file(name, std::ios::in);
    std::string src((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return src;
}

bool do_mkdir(const std::string& path) {
    struct stat st;
    if (::stat(path.c_str(), &st) != 0) {
        // Directory does not exist. EEXIST for race condition 
        if (MKDIR(path.c_str()) != 0 && errno != EEXIST) {
            return false;
        }
    } else if (!S_ISDIR(st.st_mode)) {
        errno = ENOTDIR;
        return false;
    }
    return true;
}

bool mkpath(std::string path) {
    std::string build;
    for (size_t pos = 0; (pos = path.find('/')) != std::string::npos;) {
        build += path.substr(0, pos + 1);
        do_mkdir(build);
        path.erase(0, pos + 1);
    }
    if (!path.empty()) {
        build += path;
        do_mkdir(build);
    }
    return true;
}

}