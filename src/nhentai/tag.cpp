#include <string>

#include <internal/parse.hpp>

#include <nhentai/tag.hpp>

namespace nhentai {

const Tag::Type str_to_type(const std::string& str) {
    switch (str[0]) {
        case 'p': return Tag::Type::TAG_PARODY;
        case 't': return Tag::Type::TAG_TAG;
        case 'a': return Tag::Type::TAG_ARTIST;
        case 'g': return Tag::Type::TAG_GROUP;
        case 'l': return Tag::Type::TAG_LANGUAGE;
        case 'c': {
            if (str[1] == 'h') {
                return Tag::Type::TAG_CHARACTER;
            } else {
                return Tag::Type::TAG_CATEGORY;
            }
        }
        default: return Tag::Type::TAG_UNKNOWN;
    }
}

Tag::Tag(const unsigned int _id, const std::string& _type, const std::string& _name, const std::string& _url, const unsigned int _count)
    : id(_id), type(str_to_type(_type)), name(_name), url(_url), count(_count), str(_type) {}

}