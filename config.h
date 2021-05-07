#pragma once

#define HOST_SITE "nhentai.net"
#define HOST_URL "https://" HOST_SITE
#define IMAGE_URL "https://i." HOST_SITE
#define THUMBS_URL "https://t." HOST_SITE
#define API_URL HOST_URL "/api"
#define GALLERY_URL API_URL "/gallery"
#define SEARCH_URL API_URL "/galleries/search"

/* important options */

#ifndef _WIN32
#define USE_FORKS
#endif
#define EASY_TAGS

/* 
    tag variables
    %id%, %title.pretty%, %title.english%, %title.japanese%, %media%, %parodies%, %characters%, %tags%, %artists%, %groups%, %language%, %categories%
    example below which I use
    feel free to use whatever scheme you like
*/
#define DEFAULT_NAMING "%id%"
#define DEFAULT_WORK_DIR "."

#define STATUS_COLOUR "\033[0;32m"
#define STATUS_SYMBOL '#'
#define STATUS_SIZE 50
