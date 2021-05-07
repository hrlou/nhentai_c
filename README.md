<!-- https://onlineasciitools.com/convert-text-to-ascii-art -->
                     888                        888             d8b 
                     888                        888             Y8P 
                     888                        888                 
            88888b.  88888b.   .d88b.  88888b.  888888  8888b.  888 
            888 "88b 888 "88b d8P  Y8b 888 "88b 888        "88b 888 
            888  888 888  888 88888888 888  888 888    .d888888 888 
            888  888 888  888 Y8b.     888  888 Y88b.  888  888 888 
            888  888 888  888  "Y8888  888  888  "Y888 "Y888888 888 
          
          
          
                                                        
## Purpose
**A program written entirely in C++ for downloading doujins from "https://nhentai.net"**

## Dependencies
In order to compile you need a version of the curl libraries supporting SSL.
On Debian based distributions simply install 
```
libcurl4-openssl-dev
libzip-dev
```
![curl source/binaries](https://curl.se/download.html)  
![libzip source/binaries](https://libzip.org/download/)  


## Configuration
To edit the configuration, open "config.h".  
Instructions are included in it.

## Installation
On Linux building should be as simple as.
```
git clone https://github.com/hrlou/nhentai.git
cd nhentai
sudo make install
```

## Usage
The program will download any specified doujins by their id to their own directories and/or comicbook.
```
nhentai 177013 196718 etc
```
Inside there will be a text file including the tags that looks a little like this
```
Title: title
Media_Id: id
Parodies: parody, parody
Characters: character, character
Tags: tag, tag, tag
Artists: artist, artist
Groups: group
Languages: language
Categories: category
Pages: number
```
There is also a json file included in the archive

You can search by tags like this
```
nhentai --search "foo bar"
```

## Roadmap
- [ ] cookies & option to generate them from login
- [ ] runtime config file