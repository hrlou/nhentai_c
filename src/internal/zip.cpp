#include <iostream>

#include <cstring>

#include <sys/stat.h>
#include <dirent.h>
#include <zip.h>

#include <internal/zip.hpp>
#include <internal/utils.hpp>

static void walk_directory(const std::string& startdir, const std::string& inputdir, zip_t *zipper) {
    DIR* dp = ::opendir(inputdir.c_str());
    if (dp == nullptr) {
        throw std::runtime_error("Failed to open input directory: " + std::string(::strerror(errno)));
    }

    struct dirent* dirp;
    while ((dirp = readdir(dp)) != NULL) {
        if (dirp->d_name != std::string(".") && dirp->d_name != std::string("..")) {
            std::string fullname = inputdir + "/" + dirp->d_name;
            if (utils::is_dir(fullname)) {
                if (zip_dir_add(zipper, fullname.substr(startdir.length() + 1).c_str(), ZIP_FL_ENC_UTF_8) < 0) {
                    throw std::runtime_error("Failed to add directory to zip: " + std::string(zip_strerror(zipper)));
                }
                walk_directory(startdir, fullname, zipper);
            } else {
                zip_source_t *source = zip_source_file(zipper, fullname.c_str(), 0, 0);
                if (source == nullptr) {
                    throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper)));
                }
                if (zip_file_add(zipper, fullname.substr(startdir.length() + 1).c_str(), source, ZIP_FL_ENC_UTF_8) < 0) {
                    zip_source_free(source);
                    throw std::runtime_error("Failed to add file to zip: " + std::string(zip_strerror(zipper)));
                }
            }
        }
    }
    ::closedir(dp);
}

void zip_directory(const std::string& inputdir, const std::string& output_filename) {
    int errorp;
    zip_t* zipper = zip_open(output_filename.c_str(), ZIP_CREATE | ZIP_EXCL, &errorp);
    if (zipper == nullptr) {
        zip_error_t ziperror;
        zip_error_init_with_code(&ziperror, errorp);
        throw std::runtime_error("Failed to open output file " + output_filename + ": " + zip_error_strerror(&ziperror));
    }

    try {
        walk_directory(inputdir, inputdir, zipper);
    } catch(...) {
        zip_close(zipper);
        throw;
    }

    zip_close(zipper);
}

std::string zip_get_file(const std::string& zip_name, const std::string& fname) {
    int errorp;
    zip_t* archive = zip_open(zip_name.c_str(), ZIP_CHECKCONS, &errorp);
    if (archive == nullptr) {
        zip_error_t ziperror;
        zip_error_init_with_code(&ziperror, errorp);
        throw std::runtime_error("Failed to open archive " + zip_name + ": " + zip_error_strerror(&ziperror));
    }
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(archive, fname.c_str(), 0, &st);
    if (st.size <= 0) {
        std::cerr << "Failed to open file " << fname << " in " << zip_name << std::endl;
        zip_close(archive);
        return "";
    }
    char* buf = new char[st.size];
    zip_file* f = zip_fopen(archive, fname.c_str(), 0);
    zip_fread(f, buf, st.size);
    std::string rv(buf, st.size);
    delete [] buf;
    zip_fclose(f);
    zip_close(archive);
    return rv;
}