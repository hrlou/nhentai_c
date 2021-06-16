#include <config.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include <curl/curl.h>

#include <internal/utils.hpp>
#include <internal/curl.hpp>

namespace curl {

typedef std::pair<CURL*, void*> cFile;

static void progress_bar(std::ostream& os, float numerator, float denominator) {
    float percent = ((numerator / denominator) * 100);
    os << "\r(" << (int)percent << "%) [" << STATUS_COLOUR;
    for (float i = 0; i <= STATUS_SIZE; i++) {
        if (i <= (percent / (100 / STATUS_SIZE))) {
            os << STATUS_SYMBOL;
        } else {
            os << ' ';
        }
    }
    os << "\033[0m] (" << (int)numerator << '/' << (int)denominator << ')';
    if (numerator == denominator) {
        os << std::endl;
    }
}

static int multi_download(std::vector<cFile> cfiles, bool show_progress = false, bool verbose = false) {
    curl_global_init(CURL_GLOBAL_ALL);
    CURLM* cm = curl_multi_init();
    CURLMsg* msg;
    size_t max_transfers = (MAX_PARALLEL < cfiles.size()) ? MAX_PARALLEL : cfiles.size();
    size_t transfers = 0;
    size_t progress = 1;
    int msgs_left = -1;
    int still_alive = 1;

    // Limit the amount of simultaneous connections curl should allow
    curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)max_transfers);

    for (transfers = 0; transfers < max_transfers; transfers++) {
        curl_multi_add_handle(cm, cfiles[transfers].first);
    }
    do {
        curl_multi_perform(cm, &still_alive);
        while ((msg = curl_multi_info_read(cm, &msgs_left))) {
            if (msg->msg == CURLMSG_DONE) {
                CURL* e = msg->easy_handle;
                char* url;
                curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
                if (verbose) {
                    std::cerr << "R: " << msg->data.result << " - " << curl_easy_strerror(msg->data.result) << " <" << url << '>' << std::endl;
                }
                curl_multi_remove_handle(cm, e);
                curl_easy_cleanup(e);
                if (show_progress) {
                    progress_bar(std::cerr, (float)progress++, (float)cfiles.size());
                }
            } else {
                if (verbose) {
                    std::cerr << "E: CURLMsg (" << msg->msg << ')' << std::endl;
                }
            }
            if (transfers < cfiles.size()) {
                curl_multi_add_handle(cm, cfiles[transfers++].first);
            }
        }
        if (still_alive) {
            curl_multi_wait(cm, NULL, 0, 1000, NULL);
        }
    } while (still_alive || (transfers < cfiles.size()));
    curl_multi_cleanup(cm);
    curl_global_cleanup();
    return EXIT_SUCCESS;
}

static size_t write_callback(char* buf, size_t size, size_t nmemb, std::string* src) {
    *src += std::string(buf);
    return size*nmemb;
}

namespace file {

cFile init(const std::string& url, const std::string& file) {
    CURL* curl = curl_easy_init();
    FILE* fp = ::fopen(file.c_str(), "w");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_PRIVATE, &url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
    return {curl, fp};
}

std::vector<cFile> init_vector(const std::vector<std::string>& urls, const std::vector<std::string>& files) {
    std::vector<cFile> cfiles;
    for (size_t i = 0; i < urls.size(); i++) {
        if (!utils::exist_test(files[i])) {
            cfiles.push_back(init(urls[i], files[i]));
        }
    }
    return cfiles;
}

}

namespace page {

cFile init(const std::string& url) {
    std::string* pass = new std::string;
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_PRIVATE, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, pass);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
    return {curl, pass};
}

std::vector<cFile> init_vector(const std::vector<std::string>& urls) {
    std::vector<cFile> cfiles;
    for (auto i : urls) {
        cfiles.push_back(init(i));
    }
    return cfiles;
}

}

// download file
void download(const std::string& url, const std::string& file) {
    auto cfile = file::init(url, file);
    curl_easy_perform(cfile.first);
    curl_easy_cleanup(cfile.first);
}

// download files
void download(const std::vector<std::string>& urls, const std::vector<std::string>& files) {
    auto cfiles = file::init_vector(urls, files);
    multi_download(cfiles, true);
    for (auto i : cfiles) {
        ::fclose((FILE*)i.second);
    }
}

// download page
std::string download(const std::string& url) {
    auto cpage = page::init(url);
    curl_easy_perform(cpage.first);
    curl_easy_cleanup(cpage.first);
    std::string s = *(reinterpret_cast<std::string*>(cpage.second));
    delete[] cpage.second;
    return s;
}

// download pages
std::vector<std::string> download(const std::vector<std::string>& urls) {
    auto cpages = page::init_vector(urls);
    for (auto i : urls) {
        std::cout << i << std::endl;
    }

    multi_download(cpages, false, true);
    std::vector<std::string> vc;
    vc.reserve(cpages.size());
    for (auto i : cpages) {
        std::string s = *(reinterpret_cast<std::string*>(i.second));
        delete[] i.second;
        vc.push_back(s);
    }
    return vc;
}

}