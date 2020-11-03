# nhentai-downloader
nhentai downloader written in C only dependency being curl

## installation
```
$ git clone https://github.com/hrlou/nhentai_c
$ cd nhentai_c
$ sudo make install
```

## usage
Usage is extremely simple.
```
nhentai id1 id2 id3....
```
The program will download your specified Doujins to their own directories.
It also generate a tags file named *DOUJIN-ID*.txt in said directory.   
Ex. "00000/000000.txt"
This contains all the tags and is formatted like so: 
```
Title: title
Gallery-Id: id
Parodies: parody, parody,
Characters: person, person,
Tags: tag, tag, tag,
Artists: artist
Groups: group
Languages: language
Categories: category
Pages: #page
```

## configuration
To edit the configuration, open "config.def.h".  
Instructions are included in it.

## organisation
**If you don't plan to organise your Doujins in a unified location, ignore this.**  
I personally use this system for organisation:
```
{artist,charecter,etc}/doujin
```
As long as all your Doujins are housed in a single directory, you should have no problem running the scripts. 

## scripts
The two scripts are to help you organise and share your doujin database.  
### nhentai_rename
The "nhentai_rename" script is run in a directory that contains Doujins.  
It renames everything in that directory to *DOUJIN-ID*_*YOUR-OPTION*.   
It simply reads the text file the program generates to determine tags.
## nhentai_gen
The "nhentai_gen" script allows you to generate a shell script that when run will download your entire library and organize it as you have.   
The purpose of this is to backup your library.   
You might also think about using it to:
- redownload on another device
- share your library online
- use diff to compare libraries
  
And other uses I haven't thought of

## Progress
- [x] Download Doujin
- [x] Download Doujins Tags
- [x] Name File Opon Tags
- [ ] Directory configuration with accepted tags
- [ ] Add command line options to the program, that can be enabled/disabled at compile time
- [ ] Option to run in library mode
- [ ] Option to compress to CBZ
- [ ] Option to choose a directory
- [ ] Option to search by tags
- [ ] Generate a library you can browse with thumbnails in html


