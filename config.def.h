#ifndef OPTIONS_H
#define OPTIONS_H

#define LIMIT 100

/* 
    tag variables
    $id$, $title$, $gallery$, $parodies$, $characters$, $tags$, $artists$, $groups$, $language$
    example below which I use
    feel free to use whatever scheme you like
*/
#define NAMING "$id$_$artists$"
#define MAX_DIRECTORY_LENGTH 128

/* general options 
    1 = true
    0 = false */
#define CREATE_CBZ 1
#define DELETE_DIR 1

/*  [0;31m  Red
    [0;32m  Green
    [0;33m  Yellow
    [0;34m  Blue
    [0;35m  Magenta
    [0;36m  Cyan    */
#define STATUS_COLOUR "\033[0;32m"
#define STATUS_SYMBOL '#'
#define STATUS_SIZE 50

#endif // OPTIONS_H
