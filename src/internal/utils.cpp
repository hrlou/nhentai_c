#include <sstream>
#include <fstream>

#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif

#include <ftw.h>

#include <internal/utils.hpp>

#ifdef _WIN32
#define MKDIR(PATH) ::_mkdir(PATH);
#else
#define MKDIR(PATH) ::mkdir(PATH, 0755);
#endif


#include <iostream>
#include <cstring>

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
    std::cout << "mkdir: " << path << std::endl;
    return MKDIR(path.c_str());
}

bool mkpath(const std::string& path) {
    char* copypath = ::strdup(&path[0]);
    bool status = false;
    char* pp = copypath;
    char* sp;
    while (status == false && (sp = ::strchr(pp, '/')) != 0) {
        if (sp != pp) {
            *sp = '\0';
            status = do_mkdir(copypath);
            *sp = '/';
        }
        pp = sp + 1;
    }
    if (status == true) {
        status = do_mkdir(path);
    }
    ::free(copypath);
    return status;
}

}