#pragma once

#include <fstream>

namespace nhentai {

typedef struct Tag {
    enum Type { TAG_UNKNOWN, TAG_PARODY, TAG_CHARACTER, TAG_TAG, TAG_ARTIST, TAG_GROUP, TAG_LANGUAGE, TAG_CATEGORY };
    Tag(const unsigned int _id, const std::string& _type, const std::string& _name, const std::string& _url, const unsigned int _count);

    unsigned int id;
    Type type;
    std::string name;
    std::string url;
    unsigned int count;
    std::string str;
} Tag;

const Tag::Type str_to_type(const std::string& str);

}