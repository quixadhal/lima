/* Do not remove the headers from this file! see /USAGE for more info. */

// Written by Rust.
#include <mudlib.h>

inherit OBJ;
inherit M_READABLE;
inherit M_GETTABLE;


void setup(string text)
{
    if(!strlen(text))
    {
	text = "*** Recorder heard nothing but silence.\n";
    }
  
    set_id("transcript");
    set_in_room_desc("A transcript of a recording lies here.");
    set_long("It's got a transcript of a conversation on it.\n");
    set_text(text);
}
