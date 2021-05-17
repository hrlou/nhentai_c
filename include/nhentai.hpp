#pragma once

#define HOST_SITE "nhentai.net"
#define HOST_URL "https://" HOST_SITE
#define IMAGE_URL "https://i." HOST_SITE
#define THUMBS_URL "https://t." HOST_SITE
#define API_URL HOST_URL "/api"
#define GALLERY_URL API_URL "/gallery"
#define SEARCH_URL API_URL "/galleries/search"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace nhentai {
    enum TagType { TAG_UNKNOWN, TAG_PARODY, TAG_CHARACTER, TAG_TAG, TAG_ARTIST, TAG_GROUP, TAG_LANGUAGE, TAG_CATEGORY };
    const TagType str_to_type(const std::string& str);
    const std::string type_to_str(const TagType& type);

    class Tag {
    private:
        const unsigned int m_Id;
        const std::string m_Name;
        const std::string m_Url;
        const TagType m_Type;
    public:
        Tag(const TagType type, const unsigned int id, const std::string name, const std::string url)
            : m_Id(id), m_Name(name), m_Url(url), m_Type(type) {}
        Tag(const std::string type_str, const unsigned int id, const std::string name, const std::string url)
            : m_Id(id), m_Name(name), m_Url(url), m_Type(str_to_type(type_str)) {}

        inline const unsigned int id(void) const { return m_Id; }
        inline const std::string name(void) const { return m_Name; }
        inline const std::string url(void) const { return m_Url; }
        inline const std::string type_str(void) const { return type_to_str(m_Type); }
        inline const TagType type(void) const { return m_Type; }

        inline void print(std::ostream& out) const { out << type_str() << ": " << m_Id << ", " << m_Name << "; " << m_Url; }
        
        friend std::ostream &operator<<(std::ostream& out, const Tag& obj) {
            obj.print(out);
            return out;
        };
    };

    typedef struct {
        unsigned int id;
        std::string media_id;
        struct title_struct {
            std::string english;
            std::string japanese;
            std::string pretty;
        } title;
        std::vector<char> pages;
        uint32_t upload_date = 0;
        std::vector<Tag> tags;
        size_t num_pages = 0;
    } DoujinData;

    DoujinData doujin_data_generate(json& json_obj);
    std::string doujin_data_format(std::string __fmt, std::string delim, const DoujinData data);
    void doujin_data_print(std::ostream& out, const DoujinData data);

    class Doujin {
    private:
        json m_Json;
        DoujinData m_Data;

        bool Failed = false;
        bool Downloaded = false;

        std::string m_Fmt;
        std::string m_WorkingDir;
        std::string m_OutputDir;
        std::string m_Archive;

        std::vector<std::string> m_Urls;
        std::vector<std::string> m_Files;

        inline std::string api_url(const unsigned int id) const { return std::string(GALLERY_URL) + '/' + std::to_string(id); }
        inline std::string media_url(void) const { return std::string(IMAGE_URL) + std::string("/galleries/") + m_Data.media_id + std::string("/"); }
        inline std::string page_url(size_t page, char ext) const {
            std::string page_url = media_url() + std::to_string(page);
            switch (ext) {
                case 'j': page_url += ".jpg"; break;
                case 'p': page_url += ".png"; break;
                case 'g': page_url += ".gif"; break;
            }
            return page_url;
        }

        void setup_files(void);
        void set_json(const std::string& json);
        void file_json(const std::string& name);
        void archive_json(const std::string& archive);
        void web_json(const std::string& url);
    public:
        Doujin(const json json_obj)
            : m_Json(json_obj) {
                m_Data = doujin_data_generate(m_Json);
            }
        Doujin(const std::string json_src) {
            if (json_src.front() == '{' && json_src.back() == '}') {
                set_json(json_src);
            } else if (json_src.substr(json_src.length() - 4) == "json") {
                file_json(json_src);
            } else if (json_src.substr(json_src.length() - 3) == "cbz") {
                archive_json(json_src);
            } else if (json_src.front() == '#') {
                web_json(api_url(atoi(&json_src[1])));
            } else if ((json_src.front() - '0') <= 9) {
                web_json(api_url(atoi(&json_src[0])));
            } else {
                std::cerr << "unkown source" << std::endl;
                Failed = true;
            }
        }
        Doujin(const unsigned int id) {
            web_json(api_url(id));
        }

        inline const DoujinData data(void) const { return m_Data; }
        inline const std::string dump(void) const { return m_Json.dump(4); }
        inline void print(std::ostream& out) const {
            if (Failed == true) return;
            doujin_data_print(out, m_Data);
        }

        inline void set_dir(std::string dir) { m_WorkingDir = dir; }
        inline void set_fmt(std::string fmt) { m_Fmt = fmt; }
        
        inline friend std::ostream &operator<<(std::ostream& out, const Doujin& obj) {
            obj.print(out);
            return out;
        };

        void download(void);
        void create_cbz(void);
        void remove(void);
    };

    class Search {
    private:
        const std::string m_Term;
        const std::string m_Sort;
        json m_Json;
        size_t m_NumPages = 1;
        std::vector<Doujin> m_Results;

        inline std::string arg_sort(void) { return (m_Sort.length() != 0) ? "&sort=" + m_Sort : ""; }
        inline std::string arg_page(int page) { return (page != 0) ? "&page=" + std::to_string(page) : ""; }
        inline std::string url(int page) { return std::string(SEARCH_URL) + "?query=" + m_Term + arg_sort() + arg_page(page); }

        void execute(void);
    public:
        Search(std::string term) 
            : m_Term(term) {
                execute();
            }
        Search(std::string term, std::string sort) 
            : m_Term(term), m_Sort(sort) {
                execute();
            }
        inline const std::string term(void) const { return m_Term; }
        inline const size_t size(void) const { return m_Results.size(); }
        inline const std::vector<Doujin> results(void) const { return m_Results; }
        inline const Doujin operator[](const size_t index) { return m_Results[index]; }
    };
}