/* Do not remove the headers from this file! see /USAGE for more info. */

// This file contains any #defines which specify the global
// colour schemes of the mud - this way a whole new colour
// scheme can be adopted without hunting thru a billion files

// Note that these are strings which must be fed into M_ANSI's
// ansi() function

#ifndef _COLOUR_CONFIG_H_
#define _COLOUR_CONFIG_H_

#define NORM	    			"%^RESET%^%^WHITE%^%^B_BLACK%^"

// colours for the /trans/cmds/ls command
#define LS_DIR_COLOUR      "%^BOLD%^%^BLUE%^"
#define LS_HEADER_COLOUR   "%^YELLOW%^"
#define LS_CODE_COLOUR     "%^ORANGE%^"
#define LS_PATH_COLOUR     "%^CYAN%^"
#define LS_SAVE_COLOUR  	"%^RED%^"

// colours for rooms
#define ROOM_EXIT_COLOUR		"%^MAGENTA%^"
#define ROOM_SHORT_COLOUR		"%^BLACK%^%^B_WHITE%^"

#endif
