#pragma once

#include <string>

#include <nhentai/doujin.hpp>

namespace parse {

void parse_data(nhentai::Data& data);
std::string parse_page_gallery(std::string src);
std::pair<size_t, size_t> search_size(const std::string& src);
std::vector<nhentai::Data> search_page(const std::string& src);

}