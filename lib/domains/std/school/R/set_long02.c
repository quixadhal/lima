#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Room set_long - old style");
    set_long(@MAY
The 'old style' starts and finishes with TOKENS.
By convention capitalised name, preceded by @ at the start.

set_long(@YOURWIZNAME
You type in the text that you want to say, returning each line like an
old fashioned typewriter.  This is quick and simple.  However, it imposes
your line wrapping on the player looking at the room, and his preferences
for line wrapping may be different from yours.
YOURWIZNAME
);
MAY
    );
    set_exits( ([
	"backward" : R "set_long01",
	"forward" : R "set_long03",
      ]) );
    set_objects( ([
      ]) );
}
