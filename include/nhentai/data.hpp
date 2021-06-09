#pragma once

#include <vector>
#include <string>

#include <nhentai/tag.hpp>
#include <nhentai/page.hpp>

namespace nhentai {

typedef struct Data {
    Data(void) = default;
    Data(const std::string& src);
    std::string format(std::string _fmt, std::string delim = "_") const;

    std::string json;
    unsigned int id;
    std::string media_id;
    struct title_struct {
        std::string english;
        std::string japanese;
        std::string pretty;
    } title;
    std::vector<Page> pages;
    uint32_t upload_date = 0;
    std::vector<Tag> tags;
    size_t num_pages = 0;
} Data;

void remove_duplicates(std::vector<Data>& data, const std::string fmt = "%title.pretty%");

}