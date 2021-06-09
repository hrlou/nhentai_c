#include <string>
#include <algorithm>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <internal/parse.hpp>

#include <iostream>

static nhentai::Data json_to_data(json& json) {
    nhentai::Data data;
    data.json = json.dump(4);
    if (json.at("id").type() == json::value_t::string) {
        json.at("id") = atoi(std::string(json.at("id")).c_str());
    }
    json.at("id").get_to(data.id);
    json.at("media_id").get_to(data.media_id);
    if (!json.at("title").at("english").is_null()) {
        json.at("title").at("english").get_to(data.title.english);  
    }
    if (!json.at("title").at("japanese").is_null()) {
        json.at("title").at("japanese").get_to(data.title.japanese);  
    }
    if (!json.at("title").at("pretty").is_null()) {
        json.at("title").at("pretty").get_to(data.title.pretty);  
    }
    json.at("upload_date").get_to(data.upload_date);
    for (auto i : json.at("images").at("pages")) {
        data.pages.push_back(nhentai::Page(std::string(i.at("t"))[0], i.at("w"), i.at("h")));
    }
    for (auto i : json["tags"]) {
        data.tags.push_back(nhentai::Tag(i.at("id"), i.at("type"), i.at("name"), i.at("url"), i.at("count")));
    }
    json.at("num_pages").get_to(data.num_pages);
    return data;
}

namespace parse {

void parse_data(nhentai::Data& data) {
    json json = json::parse(data.json);
    data = json_to_data(json);
}

std::string parse_page_gallery(std::string src) {
    std::string json = src.substr(src.find("JSON.parse(") + 12, src.find("\");") - 12 - src.find("JSON.parse("));
    size_t index = 0;
    for (;;) {
        index = json.find("\\u0022", index);
        if (index == std::string::npos) break;
        json.replace(index, 6, "\"");
    }
    return json;
}

std::pair<size_t, size_t> search_size(const std::string& src) {
    json json = json::parse(src);
    std::pair<size_t, size_t> size;
    size.first = json.at("num_pages").get<size_t>();
    size.second = json.at("per_page").get<size_t>();
    return size;
}

std::vector<nhentai::Data> search_page(const std::string& src) {
    json json = json::parse(src);
    std::vector<nhentai::Data> results;
    results.reserve(json["result"].size());
    for (auto i : json["result"]) {
        results.push_back(json_to_data(i));
    }
    return results;
}


}