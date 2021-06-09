#include <string>
#include <cstring>
#include <algorithm>
#include <map>

#include <internal/parse.hpp>

#include <nhentai/data.hpp>

#include <iostream>

namespace nhentai {

Data::Data(const std::string& src)
    : json(src) {
    parse::parse_data(*this);
}

std::string Data::format(std::string _fmt, std::string delim) const {
    char* fmt = &_fmt[0];
    std::string str;
    for (; *fmt; fmt++) {
        if (*fmt == '%') {
            fmt++;
            if (!::strncmp(fmt, "id", 2)) {
                str += std::to_string(id);
            } else if (!::strncmp(fmt, "media", 5)) {
                str += media_id;
            } else if (!::strncmp(fmt, "title", 5)) {
                fmt = 1 + ::strchr(fmt, '.');
                switch (*fmt) {
                    case 'p': str += title.pretty; break;
                    case 'e': str += title.english; break;
                    case 'j': str += title.japanese; break;
                }
            } else if (!::strncmp(fmt, "pages", 5)) {
                str += std::to_string(num_pages);
            } else {
                Tag::Type tag_type = str_to_type(fmt); 
                if (tag_type != Tag::Type::TAG_UNKNOWN) {
                    for (size_t i = 0; i < tags.size(); i++) {
                        if (tags[i].type == tag_type) {
                            str.append(tags[i].name);
                            str.append(delim);
                        }
                    }
                    str.erase(str.length() - delim.length(), delim.length());
                }
            }
            fmt = strchr(fmt, '%');
        } else {
            str.push_back(*fmt);
        }
    }
    return str;
}

void remove_duplicates(std::vector<Data>& data, const std::string fmt) {
    std::reverse(data.begin(), data.end());
    std::map<std::string, nhentai::Data> map;
    for (auto i : data) {
        map[i.format(fmt)] = i;
    }
    data.clear();
    for (auto const& i : map) {
        data.push_back(i.second);
    }
}

}