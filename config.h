#pragma once
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
#define TAGS_FORMAT "Title: %title.pretty%\nMedia_Id: %media_id%\nTags: %tags%\nArtists: %artists%\nGroups: %groups%\nLanguages: %languages%\nCategories: %categories%\nPages: %pages%"

#define DEFAULT_NAMING "%id%_%art%"
#define DEFAULT_WORK_DIR "."

#define STATUS_COLOUR "\033[0;32m"
#define STATUS_SYMBOL '#'
#define STATUS_SIZE 50
