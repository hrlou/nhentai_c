#include "config.h"

#include "out.hpp"
#include "curl.hpp"
#include "nhentai.hpp"
#include "utils.hpp"

#include <zip.h>

#include <iostream>
#include <vector>
#include <string>

#ifdef USE_FORKS
#include <sys/wait.h>
#include <unistd.h>
#endif

#ifdef USE_THREADS
#include <thread>
#endif

#ifndef STATUS_COLOUR
#define STATUS_COLOUR "\033[0;32m"
#endif
#ifndef STATUS_SYMBOL
#define STATUS_SYMBOL '#'
#endif
#ifndef STATUS_SIZE
#define STATUS_SIZE 50
#endif

void progress_bar(std::ostream& out, float numerator, float denominator) {
    float percent = ((numerator / denominator) * 100);
    out << "\r(" << (int)percent << "%) [" << STATUS_COLOUR;
    for (float i = 0; i <= STATUS_SIZE; i++) {
        if (i <= (percent / (100 / STATUS_SIZE))) {
            out << STATUS_SYMBOL;
        } else {
            out << ' ';
        }
    }
    out << "\033[0m] (" << (int)numerator << '/' << (int)denominator << ')';
    if (numerator == denominator) {
        out << std::endl;
    }
}

/* this is awful, it works */
#define SET_LOOP(MAX, SET_SIZE) for (size_t set_s = 0, set_e = (MAX < SET_SIZE) ? MAX : SET_SIZE; set_e <= MAX; set_s = set_e, set_e += ((MAX - set_e) <= SET_SIZE && (MAX % SET_SIZE) != 0) ? (MAX % SET_SIZE) : SET_SIZE)

void array_download(const std::string* urls, const std::string* files, size_t num) {
#if defined(USE_FORKS)
    int progress = 1;
    SET_LOOP (num, MAX_PROCESSES) {
        pid_t pid;
        for (size_t i = set_s; i < set_e; i++) {
            pid = fork();
            if (pid == 0) {
                utils::mkdir_p(utils::dirname(files[i]));
                if (!utils::exist_test(files[i])) {
                    curl::download_file(urls[i], files[i] + ".part");
                    rename((files[i] + ".part").c_str(), files[i].c_str());
                }
                exit(127);
            } else if (pid > 0) {
                // the child has been forked and we are in the parent
            } else if (pid < 0) {
                // perror("failed to fork");
                throw std::runtime_error("Failed to fork");
            }
        }
        for (int status; (pid = waitpid(-1, &status, 0)); progress++) {
            if (pid == -1) {
                break;
            } else if (pid == -1) {
                // perror("waitpid");
                throw std::runtime_error("Waitpid");
            } else if (WIFEXITED(status)) {
                progress_bar(std::cerr, progress, num);
            }
        }
    }
#elif defined(USE_THREADS)
    auto f = [](const std::string url, const std::string file) {
        utils::mkdir_p(utils::dirname(file));
        if (!utils::exist_test(file)) {
            curl::download_file(url, file + ".part");
            rename((file + ".part").c_str(), file.c_str());
        }
    };
    std::vector<std::thread> thread_vector;
    size_t progress = 1;
    SET_LOOP (num, MAX_PROCESSES) {
        for (size_t i = set_s; i < set_e; i++) {
            thread_vector.push_back(std::thread(f, urls[i], files[i]));
        }
        for (size_t i = 0; i < thread_vector.size(); i++) {
            thread_vector.at(i).join();
            progress_bar(std::cerr, progress++, num);
        }
        thread_vector.clear();
    }
#else
    for (size_t i = 0; i < num; i++) {
        utils::mkdir_p(utils::dirname(files[i]));
        if (!utils::exist_test(files[i])) {
            curl::download_file(urls[i], files[i] + ".part");
            rename((files[i] + ".part").c_str(), files[i].c_str());
        }
        progress_bar(std::cerr, i + 1, num);
    }
#endif
}