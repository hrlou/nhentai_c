#include <iostream>
#include <fstream>
#include <vector>
#include <future>

#include <getopt.h>

#include <nhentai/doujin.hpp>
#include <nhentai/search.hpp>

struct option_struct {
    std::string fmt;
    std::string output_dir;
    bool duplicates = false;
    bool inspect = false;
    bool print = false;
    bool download = true;
    bool cbz = true;
    bool remove = true;
};

static struct option_struct OPTIONS;

static void handle_nhentai(nhentai::Doujin doujin) {
    if (OPTIONS.inspect) {
        std::cerr << "Download [Y/N]: ";
        char opt;
        std::cin >> opt;
        opt = toupper(opt);
        if (opt == 'N') {
            return;
        }
    }
    if (OPTIONS.output_dir.length() != 0) {
        doujin.output_dir = OPTIONS.output_dir;
    }
    if (OPTIONS.fmt.length() != 0) {
        doujin.fmt = OPTIONS.fmt;
    }
    if (OPTIONS.print) {
        std::cout << doujin;
    }
    if (OPTIONS.download) {
        if (!doujin.download()) {
            return;
        }
    }
    if (OPTIONS.cbz) {
        doujin.create_cbz();
    }
    if (OPTIONS.remove) {
        doujin.remove();
    }
}

static void handle_nhentai_array(std::vector<nhentai::Doujin> doujins) {
    size_t count = doujins.size();
    for (size_t i = 0; i < count; i++) {
        std::cerr << "Downloading (" << i + 1 << '/' << count << ") " << doujins[i].data.id << " : " << doujins[i].data.title.pretty << std::endl;
        handle_nhentai(doujins[i]);
    }
}

static void handle_search(const std::string search_term) {
    auto results = nhentai::search(search_term);
    if (!OPTIONS.duplicates) {
    const size_t orig = results.size();
        std::cerr << "removing duplicates ";
        nhentai::remove_duplicates(results);
    std::cerr << (orig - results.size()) << std::endl;
        std::cerr << results.size() << " results for \'" << search_term << '\'' << std::endl;
    } else {
        std::cerr << results.size() << " results for \'" << search_term << '\'' << std::endl;
    }
    handle_nhentai_array(std::vector<nhentai::Doujin>(results.begin(), results.end()));
}

static void print_banner(std::ostream& out) {
    out << "nhentai downloader C++" << std::endl;
    out << " -h, --help\tDisplay this help text" << std::endl;
    out << " -s, --search\tSearch term" << std::endl;
    out << " -l, --list\tList of ids seperated by hashes" << std::endl;
    out << " -o, --output\tOutput directory" << std::endl;
    out << " -f, --format\tFormat saved doujin name" << std::endl;
    out << " -I  --inspect\tInspect each doujin" << std::endl;
    out << " -D, --duplicates\tKeep duplicates" << std::endl;
    out << " -p, --print\tDisplay doujin tags" << std::endl;
    out << " -N, --nono\tSkip doujin download" << std::endl;
    out << " -C, --cbz\tCreate cbz from doujin" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        print_banner(std::cerr);
        exit(0);
    }
    std::string search_term;
    std::string id_list;
    while (1) {
        int option_index = 0, c;
        static struct option long_options[] = {
            {"search",      required_argument,  0,  's'},
            {"list",        required_argument,  0,  'l'},
            {"output",      required_argument,  0,  'o'},
            {"format",      required_argument,  0,  'f'},
            {"help",        no_argument,        0,  'h'},
            {"inspect",     no_argument,        0,  'I'},
            {"duplicates",  no_argument,        0,  'D'},
            {"print",       no_argument,        0,  'p'},
            {"nono",        no_argument,        0,  'N'},
            {"cbz",         no_argument,        0,  'C'},
            {0,             0,                  0,  0}
        };

        c = getopt_long(argc, argv, "s:l:o:f:hIDpNC", long_options, &option_index);
        if (c == -1) {
            break;
        }
        switch (c) {
        case 's': search_term = optarg; break;
        case 'l': id_list = optarg; break;
        case 'o': OPTIONS.output_dir = optarg; break;
        case 'f': OPTIONS.fmt = optarg; break;
        case 'I': OPTIONS.inspect = true; break;
        case 'D': OPTIONS.duplicates = true; break;
        case 'h': print_banner(std::cerr); break;
        case 'p': OPTIONS.print = true;  break;
        case 'N': OPTIONS.download = false; break;
        case 'C': OPTIONS.cbz = true; break;
        default:
            std::cerr << "[getopt] returned character code 0 " << c << std::endl;
            print_banner(std::cerr);
            exit(0);
        }
    }

    if (search_term.length() != 0) {
        handle_search(search_term);
    }

    std::vector<nhentai::Doujin> arg_doujins;
    for (int index = optind; index < argc; index++) {
        arg_doujins.push_back(nhentai::Doujin(std::string(argv[index])));
    }
    if (id_list.length() != 0) {
        if (id_list.back() != '#') {
            id_list.push_back('#');
        }
        if (id_list.front() == '#') {
            id_list.erase(0, 1);
        }
        for (size_t pos = 0; (pos = id_list.find('#')) != std::string::npos;) {
            arg_doujins.push_back(nhentai::Doujin(id_list.substr(0, pos)));
            id_list.erase(0, pos + 1);
        }
    }
    handle_nhentai_array(arg_doujins);
    return 0;
}
