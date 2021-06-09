#pragma once

#include <nhentai/data.hpp>

#include <string>
#include <vector>
#include <fstream>

namespace nhentai {
    
class Doujin {
public:
    Doujin(const std::string& _src);
    Doujin(const Data& _data) {
        data = _data;
    }
    friend std::ostream& operator<<(std::ostream& os, const Doujin& doujin);
    void download(void);
    void remove(void);
    void create_cbz(void);
public:
    Data data;
    std::string fmt;
    std::string output_dir;
    std::string doujin_dir;
private:
    void setup_files(void);
private:
    bool created_cbz = false;
    bool created_dir = false;
};

}