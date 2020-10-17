#ifndef OPTIONS_H
#define OPTIONS_H

typedef enum {true, false} bool;
/*	naming scheme
	ex. doujinid_NAMING
	NULL, Parodies, Characters
	Tags, Artists, Groups
	Languages, Categories, Pages
	NOTE, SETTING THIS TO A CLASS WITH A LOT OF TAGS
	I.E. "Tags", WILL CAUSE THE PROGRAM TO CRASH!
	THE OPTION IS STILL THERE, BUT BE WARNED
*/
const char NAMING[] = "Artists";
// search feature not yet implemented! for later
/*	Self Explanitory
	NULL = Search all languages*/
// #define LANGUAGE "Japanese"

#endif // OPTIONS_H