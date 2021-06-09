#pragma once

#include <string>
#include <fstream>

namespace nhentai {

typedef struct Page {
    Page(const char _t, const unsigned int _w, const unsigned int _h);
    std::string ext(void) const;

    char t;
    unsigned int w;
    unsigned int h;
} Page;

}