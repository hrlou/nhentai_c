#include <string>

#include <nhentai/page.hpp>

namespace nhentai {

Page::Page(const char _t, const unsigned int _w, const unsigned int _h)
    : t(_t), w(_w), h(_h) {}

std::string Page::ext(void) const {
    switch (t) {
        case 'j': return "jpg";
        case 'p': return "png";
        case 'g': return "gif";
    }
    return "";
}

}