#include "config.h"

#include "nhentai.hpp"
#include "curl.hpp"
#include "out.hpp"
#include "utils.hpp"

#include <iostream>
#include <fstream>
#include <climits>
#include <algorithm>

#include "zip.hpp"

#ifndef NAME_MAX
#define NAME_MAX 128
#endif

namespace nhentai {
    const TagType str_to_type(const std::string& str) {
        switch (str[0]) {
            case 'p': return TAG_PARODY;
            case 't': return TAG_TAG;
            case 'a': return TAG_ARTIST;
            case 'g': return TAG_GROUP;
            case 'l': return TAG_LANGUAGE;
            case 'c': {
                if (str[1] == 'h') {
                    return TAG_CHARACTER;
                } else {
                    return TAG_CATEGORY;
                }
            }
            default: return TAG_UNKNOWN;
        }
    }

    const std::string type_to_str(const TagType& type) {
        switch (type) {
            case TAG_UNKNOWN: return "UNKNOWN";
            case TAG_PARODY: return "PARODY";
            case TAG_CHARACTER: return "CHARACTER";
            case TAG_TAG: return "TAG";
            case TAG_ARTIST: return "ARTIST";
            case TAG_GROUP: return "GROUP";
            case TAG_LANGUAGE: return "LANGUAGE";
            case TAG_CATEGORY: return "CATEGORY";
        }
        throw std::runtime_error("TagType unset");
    }
}

namespace nhentai {
    DoujinData doujin_data_generate(json& json_obj) {
        DoujinData data;
        if (json_obj.at("id").type() == json::value_t::string) {
            json_obj.at("id") = atoi(std::string(json_obj.at("id")).c_str());
        }
        json_obj.at("id").get_to(data.id);
        json_obj.at("media_id").get_to(data.media_id);
        if (!json_obj.at("title").at("english").is_null()) {
            json_obj.at("title").at("english").get_to(data.title.english);  
        }
        if (!json_obj.at("title").at("japanese").is_null()) {
            json_obj.at("title").at("japanese").get_to(data.title.japanese);  
        }
        if (!json_obj.at("title").at("pretty").is_null()) {
            json_obj.at("title").at("pretty").get_to(data.title.pretty);  
        }
        json_obj.at("upload_date").get_to(data.upload_date);
        for (auto i : json_obj.at("images").at("pages")) {
            data.pages.push_back(i.at("t").get<std::string>()[0]);
        }
        for (auto i : json_obj["tags"]) {
            data.tags.push_back(Tag((std::string)i.at("type"), i.at("id"), i.at("name"), i.at("url")));
        }
        json_obj.at("num_pages").get_to(data.num_pages);
        return data;
    }

    std::string doujin_data_format(std::string __fmt, std::string delim, const DoujinData data) {
        char* fmt = &__fmt[0];
        std::string str;
        for (; *fmt; fmt++) {
            if (*fmt == '%') {
                fmt++;
                if (!strncmp(fmt, "id", 2)) {
                    str += std::to_string(data.id);
                } else if (!strncmp(fmt, "media", 5)) {
                    str += data.media_id;
                } else if (!strncmp(fmt, "title", 5)) {
                    fmt = 1 + strchr(fmt, '.');
                    switch (*fmt) {
                        case 'p': str += data.title.pretty; break;
                        case 'e': str += data.title.english; break;
                        case 'j': str += data.title.japanese; break;
                    }
                } else if (!strncmp(fmt, "pages", 5)) {
                    str += std::to_string(data.num_pages);
                } else {
                    TagType tag_type = str_to_type(fmt);
                    if (tag_type != TAG_UNKNOWN) {
                        for (size_t i = 0; i < data.tags.size(); i++) {
                            if (data.tags[i].type() == tag_type) {
                                str.append(data.tags[i].name());
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

    void doujin_data_print(std::ostream& out, const DoujinData data) {
        out << doujin_data_format(TAGS_FORMAT, ", ", data) << std::endl;
    }
}

namespace nhentai {
    void Doujin::set_json(const std::string& json) {
        if (json.front() != '{' && json.back() != '}') {
            std::cerr << "invalid json data, skipping" << std::endl;
            m_Failed = true;
        } else {
            m_Json = json::parse(json);
            m_Data = doujin_data_generate(m_Json);
        }                
    }

    void Doujin::file_json(const std::string& name) {
        std::ifstream file(name, std::ios::in);
        std::string src((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        set_json(src);
    }

    void Doujin::archive_json(const std::string& archive) {
        set_json(zip_get_file(archive, "index.json"));
    }
    
    void Doujin::web_json(const std::string& url) {
        std::string json = curl::download_html(url);
        if (json.length() <= 0) {
            std::cerr << "couldn't download json, skipping" << std::endl;
            m_Failed = true;
        } else {
            set_json(json);
        }
    }

    void Doujin::setup_files(void) {
        if (m_WorkingDir.empty()) {
            m_WorkingDir = DEFAULT_WORK_DIR;
        }
        if (m_Fmt.empty()) {
            m_Fmt = DEFAULT_NAMING;
        }
        if (m_WorkingDir.back() != '/') {
            m_WorkingDir.push_back('/');
        }
        std::string tmp = doujin_data_format(m_Fmt, "_", m_Data);
        std::replace(tmp.begin(), tmp.end(), ' ', '-');
        while (tmp.back() == '/') {
            tmp.pop_back();
        }
        tmp.push_back('/');
        std::vector<std::string> tokens;
        for (size_t pos = 0; (pos = tmp.find('/')) != std::string::npos;) {
            tokens.push_back(tmp.substr(0, pos));
            tmp.erase(0, pos + 1);
        }
        for (auto i : tokens) {
            m_OutputDir += i.substr(0, (i.length() < NAME_MAX) ? i.length() : NAME_MAX) + '/';
        }
        m_OutputDir.insert(0, m_WorkingDir);
        for (auto i : m_Data.pages) {
            m_Urls.push_back(page_url(m_Urls.size() + 1, i));
        }
        for (size_t i = 0; i < m_Data.num_pages; i++) {
            m_Files.push_back(utils::strformat("%s%03d.%s", m_OutputDir.c_str(), i + 1, m_Urls[i].substr(m_Urls[i].length() - 3, m_Urls[i].length()).c_str()));
        }
    }

    void Doujin::download(void) {
        if (m_Failed == true) {
            return;
        }
        setup_files();
        std::string archive = m_OutputDir.substr(0, m_OutputDir.length() - 1) + ".cbz";
        if (utils::exist_test(archive)) {
            std::cerr << "Skipping " << archive << ": File already exists" << std::endl;
            return;
        }
        vector_download(m_Urls, m_Files);
        m_Files.push_back((m_OutputDir + "index.json"));
        std::ofstream(m_Files.back(), std::ios::out) << m_Json.dump();
        m_Files.push_back((m_OutputDir + std::to_string(m_Data.id) + ".txt"));
        std::ofstream(m_Files.back(), std::ios::out) << doujin_data_format(TAGS_FORMAT, ", ", m_Data);
        zip_directory(m_OutputDir, archive);
        if (m_RemoveDir) {
            for (auto i : m_Files) {
                ::remove(i.c_str());
            }
            ::remove(m_OutputDir.c_str());
        }
    }
}

namespace nhentai {
    void Search::eat_json(void) {
        for (auto i : m_Json["result"]) {
            m_Results.push_back(Doujin(i));
        }
    }

    void Search::execute(void) {
        for (size_t i = 1; i <= m_NumPages; i++) {
            m_Json = json::parse(curl::download_html(url(i)));
            if (i == 1) {
                m_NumPages = m_Json["num_pages"];
                if (m_NumPages == 0) {
                    break;
                }
            }
            eat_json();
        }
    }
}