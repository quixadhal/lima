#include <mudlib.h>
inherit OBJ;
inherit M_READABLE;
inherit M_GETTABLE;

create()
{
    object::create();
    set_id("sign");
set_gettable(0);

  set_in_room_desc("A small sign");

set_long("Perhaps you should read it"); 
set_text( @ENDMARKER
Welcome to the wonderful world of Lima Bean, created by Rust,
Deathblade and Beek.  This rooms and the objects here are designed to
be examples of how to effectively use the Lima mudlib.  Please feel
free to make comments and suggestions.
ENDMARKER );   

}

