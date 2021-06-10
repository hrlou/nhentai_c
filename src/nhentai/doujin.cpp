#include <config.h>

#include <iostream>
#include <fstream>
#include <algorithm>

#include <climits>
#ifndef NAME_MAX
#define NAME_MAX 128
#endif

#include <internal/parse.hpp>
#include <internal/curl.hpp>
#include <internal/utils.hpp>
#include <internal/zip.hpp>
#include <internal/download.hpp>

#include <nhentai/data.hpp>
#include <nhentai/doujin.hpp>

namespace gen {

static std::string url_api(const unsigned int id) {
    return std::string(GALLERY_URL) + '/' + std::to_string(id); 
}

static std::string url_media(const nhentai::Data& data) {
    return std::string(IMAGE_URL) + std::string("/galleries/") + data.media_id + std::string("/");
}

static std::string url_page(const nhentai::Data& data, const size_t page) {
    return url_media(data) + std::to_string(page) + '.' + data.pages[page - 1].ext();
}

static std::vector<std::string> page_urls(const nhentai::Data& data) {
    std::vector<std::string> urls;
    for (size_t i = 1; i <= data.pages.size(); i++) {
        urls.push_back(url_page(data, i));
    } 
    return urls;
}

static std::vector<std::string> page_files(const nhentai::Data& data, const std::string prefix = "") {
    std::vector<std::string> pages;
    for (size_t i = 1; i <= data.pages.size(); i++) {
        pages.push_back(prefix + std::string("000", 3 - std::to_string(i).size()) + std::to_string(i) + '.' + data.pages[i - 1].ext());
    } 
    return pages;
}

}

// public
namespace nhentai {

Doujin::Doujin(const std::string& _src) {
    std::string src;
    if (_src.front() == '{' && _src.back() == '}') {
        src = _src;
    } else if (_src.substr(_src.length() - 4) == "json") {
        src = utils::read_file(_src);
    } else if (_src.substr(_src.length() - 3) == "cbz") {
        src = zip_get_file(_src, "index.json");
    } else if ((_src.front() - '0') <= 9) {
        src = curl::download_page(gen::url_api(::atoi(_src.c_str())));
    } else if (_src.substr(0, 8) == "https://") {
        src = parse::parse_page_gallery(curl::download_page(_src));
    }

    if (src.empty()) {
        std::cerr << "[Doujin] Invalid input \"" << _src << "\": Cannot initialise" << std::endl;
        exit(1);
    } else {
        data = Data(src);
    }
}

std::ostream& operator<<(std::ostream& os, const Doujin& doujin) {
    return os << doujin.data.format(TAGS_FORMAT, ", ");
}

bool Doujin::download(void) {
    setup_files();
    if (utils::exist_test(doujin_dir.substr(0, doujin_dir.length() - 1) + ".cbz")) {
        std::cerr << "[Doujin] Skipping \"" << data.id << "\": File already exists" << std::endl;
        created_cbz = true;
        return false;
    }
    download_vector(gen::page_urls(data), gen::page_files(data, doujin_dir));
    std::ofstream((doujin_dir + "index.json"), std::ios::out) << data.json;
    std::ofstream((doujin_dir + std::to_string(data.id) + ".txt"), std::ios::out) << *this;
    created_dir = true;
    return true;
}

bool Doujin::remove(void) {
    if (created_dir) {
        for (auto i : gen::page_files(data, doujin_dir)) {
            ::remove(i.c_str());
        }
        ::remove((doujin_dir + "index.json").c_str());
        ::remove((doujin_dir + std::to_string(data.id) + ".txt").c_str());
        ::remove(doujin_dir.c_str());
	return true;
    } else {
        std::cerr << "[Doujin] Cannot delete doujin: doujin not downloaded" << std::endl;
	return false;
    }
}

bool Doujin::create_cbz(void) {
    if (!created_dir) {
        std::cerr << "[Doujin] Cannot create cbz file: doujin not downloaded" << std::endl;
        return false;
    } else if (created_cbz) {
        std::cerr << "[Doujin] Cannot create cbz file: already exists" << std::endl;
    	return false;
    } else {
        zip_directory(doujin_dir, (doujin_dir.substr(0, doujin_dir.length() - 1) + ".cbz"));
    }
    return true;
}

}

// private 
namespace nhentai {

void Doujin::setup_files(void) {
    // set defaults
    if (output_dir.empty()) {
        output_dir = DEFAULT_WORK_DIR;
    }
    if (fmt.empty()) {
        fmt = DEFAULT_NAMING;
    }
    // needs to end in '/'
    if (output_dir.back() != '/') {
        output_dir.push_back('/');
    }
    std::string tmp = data.format(fmt);
    // prevent subdirectory names from being too long
    std::replace(tmp.begin(), tmp.end(), ' ', '-');
    while (tmp.back() == '/') {
        tmp.pop_back();
    }
    tmp.push_back('/');
    std::vector<std::string> tokens;
    for (size_t pos = 0; (pos = tmp.find('/')) != std::string::npos;) {
        tokens.push_back(tmp.substr(0, pos));
        tmp.erase(0, pos + 1);
    }
    for (auto i : tokens) {
        doujin_dir += i.substr(0, (i.length() < NAME_MAX) ? i.length() : NAME_MAX - 12) + '/';
    }
    doujin_dir.insert(0, output_dir);
}

}
