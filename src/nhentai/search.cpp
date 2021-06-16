#include <config.h>

#include <string>
#include <algorithm>

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

#include <iostream>
namespace nhentai {

std::vector<Data> search(const std::string query, const std::string sort) {
    std::string p1 = curl::download(gen::url(query, sort, 0));
    // first = num pages, second = per page
    auto size = parse::search_size(p1);
    if (size.first == 0) {
        return {};
    }
    
    // asynchronously download the pages
    std::vector<std::string> urls;
    urls.reserve(size.first - 1);
    for (size_t i = 2; i <= size.first; i++) {
        urls.push_back(gen::url(query, sort, i));
    }
    std::vector<std::string> pages;
    
    // setup pages vector
    pages.push_back(p1);
    auto a = curl::download(urls);
    pages.reserve(size.first);
    pages.insert(pages.end(), a.begin(), a.end());
    std::reverse(pages.begin(), pages.end());

    // parse results
    std::vector<Data> results;
    results.reserve(size.second * size.first);
    for (auto i : pages) {
        auto parse = parse::search_page(i);
        results.insert(results.begin(), parse.begin(), parse.end());
    }
    return results;
}

}
