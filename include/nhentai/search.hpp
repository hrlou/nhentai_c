#pragma once

#include <string>
#include <vector>

namespace nhentai {

std::vector<Data> search(const std::string query, const std::string sort = "popular");

}