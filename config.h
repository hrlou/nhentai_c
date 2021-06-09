#pragma once
#define USE_THREADS
#define MAX_PROCESSES 25

#define HOST_SITE "nhentai.net"
#define HOST_URL "https://" HOST_SITE
#define IMAGE_URL "https://i." HOST_SITE
#define THUMBS_URL "https://t." HOST_SITE
#define API_URL HOST_URL "/api"
#define GALLERY_URL API_URL "/gallery"
#define SEARCH_URL API_URL "/galleries/search"

// tag variables
// %id%, %media%, %title.pretty%, %title.english%, %title.japanese%, %parodies%, %characters%, %tags%, %artists%, %groups%, %language%, %categories%

// default directory/comicbook naming scheme
#define DEFAULT_NAMING "%id%_%art%"
// default directory to work in
#define DEFAULT_WORK_DIR "."
// format to print DoujinData in
#define TAGS_FORMAT "Title: %title.pretty%\nMedia_Id: %media_id%\nTags: %tags%\nArtists: %artists%\nGroups: %groups%\nLanguages: %languages%\nCategories: %categories%\nPages: %pages%"

#define MAX_PROCESSES 25

#define STATUS_COLOUR "\033[0;32m"
#define STATUS_SYMBOL '#'
#define STATUS_SIZE 50