#include <config.h>

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include <internal/curl.hpp>
#include <internal/utils.hpp>
#include <internal/download.hpp>

#ifdef USE_THREADS
#include <thread>
#endif

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

/* this is awful, it works */
#define SET_LOOP(MAX, SET_SIZE) for (size_t set_s = 0, set_e = (MAX < SET_SIZE) ? MAX : SET_SIZE; set_e <= MAX; set_s = set_e, set_e += ((MAX - set_e) <= SET_SIZE && (MAX % SET_SIZE) != 0) ? (MAX % SET_SIZE) : SET_SIZE)

void download_array(const std::string* urls, const std::string* files, size_t num) {
#if defined(USE_THREADS)
    std::vector<std::thread> thread_vector;
    size_t progress = 1;
    SET_LOOP (num, MAX_PROCESSES) {
        for (size_t i = set_s; i < set_e; i++) {
            thread_vector.push_back(std::thread(curl::download_file, urls[i], files[i]));
        }
        for (size_t i = 0; i < thread_vector.size(); i++) {
            thread_vector.at(i).join();
            progress_bar(std::cerr, progress++, num);
        }
        thread_vector.clear();
    }
#else
    for (size_t i = 0; i < num; i++) {
        curl::download_file(urls[i], files[i]);
        std::cout << files[i] << std::endl;
    }
#endif
}