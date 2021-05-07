#define _XOPEN_SOURCE 500
#include <vector>
#include <iostream>
#include <fstream>
#include <getopt.h>

#include "nhentai.hpp"

struct option_struct {
    std::string fmt;
    std::string dir;
    bool print = false;
    bool nono = false;
};

static void handle_nhentai(nhentai::Doujin doujin, struct option_struct opts) {
    if (opts.dir.length() != 0) {
        doujin.set_dir(opts.dir);
    }
    if (opts.fmt.length() != 0) {
        doujin.set_fmt(opts.fmt);
    }
    if (opts.print) {
        std::cout << doujin;
    }
    if (!opts.nono) {
        doujin.download();
    }
}

static void handle_nhentai_array(std::vector<nhentai::Doujin> doujins, struct option_struct opts) {
    size_t count = doujins.size();
    for (size_t i = 0; i < count; i++) {
        std::cerr << "Downloading (" << i + 1 << '/' << count << ") " << doujins[i].data().id << " : " << doujins[i].data().title.pretty << std::endl;
        handle_nhentai(doujins[i], opts);
    }
}

static void handle_search_inspect(nhentai::Search search, struct option_struct opts) {
    for (size_t i = 0; i < search.size(); i++) {
        std::cerr << '(' << i + 1 << '/' << search.size() << ") " << search[i].data().id << std::endl;
        std::cerr << search[i] << std::endl;
        for (;;) {
            std::cerr << "Download [Y/N]: ";
            char opt;
            std::cin >> opt;
            opt = toupper(opt);
            if (opt == 'Y') {
                handle_nhentai(search[i], opts);
                break;
            } else if (opt == 'N') {
                break;
            } else {
                std::cerr << "Invalid Argument" << std::endl;
            }
        }
        std::cerr << std::endl;
    }
}

static void handle_search(nhentai::Search search, struct option_struct opts) {
    std::cerr << search.size() << " results for \'" << search.term() << '\'' << std::endl;
    for (;;) {
	std::cerr << "\tA. Download All" << std::endl << "\tI. Inspect All" << std::endl << "\tC. Cancel\n[A/I/C]: ";
        char opt = toupper(getchar());
        getchar();
        switch (opt) {
            case 'A': return handle_nhentai_array(search.results(), opts);
            case 'I': return handle_search_inspect(search, opts);
            case 'C': return;
	    default: std::cerr << "Invalid Argument";
        }
    }
}

int main(int argc, char *argv[]) {
    std::string search_term;
    struct option_struct opts;
    while (1) {
        int option_index = 0, c;
        static struct option long_options[] = {
            {"search",      required_argument,  0,  's'},
            {"dir",         required_argument,  0,  'd'},
            {"name",        required_argument,  0,  'n'},
            {"update",      required_argument,  0,  'u'},
            {"nono",        no_argument,        0,  'N'},
            {"print",       no_argument,        0,  'p'},
            {"help",        no_argument,        0,  'h'},
            {0,             0,                  0,  0}
        };

        c = getopt_long(argc, argv, "s:d:n:u:Nph", long_options, &option_index);
        if (c == -1) {
            break;
        }
        switch (c) {
            case 's':
                search_term = optarg;
                break;
            case 'd':
                opts.dir = optarg;
                break;
            case 'n':
                opts.fmt = optarg;
                break;
	    case 'N':
		opts.nono = true;
		break;
	    case 'p':
		opts.print = true;
		break;
            case 'h':
		std::cerr << "Help!" << std::endl;
                break;
            default:
		std::cerr << "[getopt] returned character code 0 " << c << std::endl;
        }
    }

    if (search_term.length() != 0) {
        nhentai::Search arg_search(search_term, "popular");
        handle_search(arg_search, opts);
    }

    std::vector<nhentai::Doujin> arg_doujins;
    for (int index = optind; index < argc; index++) {
        arg_doujins.push_back(nhentai::Doujin(std::string(argv[index])));
    }
    handle_nhentai_array(arg_doujins, opts);
    return 0;
}
