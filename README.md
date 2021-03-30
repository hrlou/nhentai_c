<!-- https://onlineasciitools.com/convert-text-to-ascii-art -->
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
libzip-dev
```
![curl source/binaries](https://curl.se/download.html)  
![libzip source/binaries](https://libzip.org/download/)  


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
The program will download any specified doujins by their id to their own directories and/or comicbook.
```
nhentai 177013 196718 etc
```
Inside there will be a text file including the tags that looks a little like this
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
You can search for tags like this
```
nhentai --search "yuri english etc"
```
Currently this will just download the results, I will write a proper front-end

## Configuration
To edit the configuration, open "config.def.h".  
Instructions are included in it.

## Roadmap & Progress
- [ ] cookies & option to generate them from login
- [ ] option to choose a directory