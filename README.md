
                     888                        888             d8b 
                     888                        888             Y8P 
                     888                        888                 
            88888b.  88888b.   .d88b.  88888b.  888888  8888b.  888 
            888 "88b 888 "88b d8P  Y8b 888 "88b 888        "88b 888 
            888  888 888  888 88888888 888  888 888    .d888888 888 
            888  888 888  888 Y8b.     888  888 Y88b.  888  888 888 
            888  888 888  888  "Y8888  888  888  "Y888 "Y888888 888 
          
          
          
                                                        
[![Total alerts](https://img.shields.io/lgtm/alerts/g/hrlou/nhentai_c.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/hrlou/nhentai_c/alerts/)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/hrlou/nhentai_c.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/hrlou/nhentai_c/context:cpp)
![Lines: C/C++](https://badgen.net/lgtm/lines/g/hrlou/nhentai_c/c)
![Languages](https://img.shields.io/badge/language-C-%23A8B9CC?logo=c)
![Code Style](https://badgen.net/badge/code%20style/K&R/f2a)

## Purpose
**A program written entirely in C for downloading doujins from "https://nhentai.net"**

## Dependencies
In order to compile you need a version of the curl libraries supporting SSL.
On Debian based distributions simply install 
```
libcurl4-openssl-dev
```
Or you can find the source/binaries ![here](https://curl.se/download.html)

## Installation
On Linux building should be as simple as.
```
git clone https://github.com/hrlou/nhentai_c.git
cd nhentai_c
sudo make install
```
I personally use GCC 10 as my compiler, but I've also tested with clang and tcc which both seem to work fine.

I'm not very experienced when it comes to development for Windows and was unable to produce a functional binary.
Contribution to make this possible would be greatly appreciated.

## Usage
The program will download any specified doujins by their id to their own directories.
```
nhentai 177013 196718..
```
The program should theoretically support an unlimited amount of doujins at once, but I have had errors downloading over 50.
Your mileage may vary.
It also generate a tags file named *DOUJIN-ID*.txt inside of said directory containing all the tags, it looks like this.
```
Title: title
Gallery-Id: id
Parodies: parody, parody
Characters: character, character
Tags: tag, tag, tag
Artists: artist, artist
Groups: group
Languages: language
Categories: category
Pages: number
```

## Configuration
To edit the configuration, open "config.def.h".  
Instructions are included in it.

## Scripts
All scripts will eventually be rewritten as C functions and called by command line arguments.
### nhentai_cbz
Compresses all doujins in a directory to cbz files for your comic reader.
### nhentai_search
A prototype for the search function.
Takes a query and returns every doujin matching it.
The query is the same as what you'd input into the website.
For example
```
nhentai_search "yuri english"
```
### nhentai_rename
Renames all doujins in a directory with the option provided.
### nhentai_gen
The "nhentai_gen" script allows you to generate a shell script that downloads your entire library, copying the directory structure.   

## Functionality
The program goes through 6 steps in order to download a doujin.
1. Download the web page of the given id, "https://nhentai/g/*id*/"
2. Sanitise that data for our parser
3. Manually parse the data getting our tags
4. Store those tags in our struct defined as type ntags
5. Use the newly found gallery id to fork download all the images in the doujin
6. Dump the tags to a file

## Roadmap & Progress
- [x] download doujin
- [x] download doujins tags
- [x] name file upon tags
- [x] proper exist testing (needs improvement)
- [x] command line arguments (needs improvement)
- [ ] cookies
- [ ] proper searching
- [ ] proper compression for 
- [ ] option to choose a directory
- [ ] elimination of all heap memory, mallocs and pointers (will require a complete redesign)
