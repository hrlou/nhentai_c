#include <config.h>

#include <string>
#include <algorithm>
#include <iostream>

#include <internal/parse.hpp>
#include <internal/curl.hpp>

#include <nhentai/search.hpp>

namespace gen {
    static inline std::string arg_sort(const std::string& sort) {
        return (sort.length() != 0) ? "&sort=" + sort : "";
    }

    static inline std::string arg_page(const int page) {
        return (page != 0) ? "&page=" + std::to_string(page) : "";
    }

    static inline std::string arg_query(const std::string& query) {
        return "?query=" + query;
    }

    static inline std::string url(const std::string& query, const std::string& sort, int page) {
        return std::string(SEARCH_URL) + gen::arg_query(query) + gen::arg_sort(sort) + gen::arg_page(page);
    }

}

namespace nhentai {

std::vector<Data> search(const std::string query, const std::string sort) {
    std::vector<std::string> pages;
    pages.push_back(curl::download_page(gen::url(query, sort, 0)));
    std::pair<size_t, size_t> size = parse::search_size(pages.front());
    if (size.first == 0) {
        std::cerr << "[search] no results for: " << query << std::endl;
        return {};
    }
    
    pages.reserve(size.first);
    for (size_t i = 2; i <= size.first; i++) {
        pages.push_back(curl::download_page(gen::url(query, sort, i)));
    }
    std::reverse(pages.begin(), pages.end());

    std::vector<Data> results;
    results.reserve(size.second * size.first);
    for (auto i : pages) {
        auto parse = parse::search_page(i);
        results.insert(results.begin(), parse.begin(), parse.end());
    }
    return results;
}

}