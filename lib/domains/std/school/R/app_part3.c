#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Area Construction -- part 3");
    set_long(@MAY
Guidelines for descriptions of ANY kind:

1) They must be colorful, wordwise, not colorful ANSI-wise: ANSI colored words
don't always show up the same on everybody's screen the same. Remember that
when coding ANYTHING with color if color is allowed in what you are coding.

2) Descriptions are for painting pictures in the mind: Think of it as telling a
short story about a room, item, etc. Remember, while writing it, how your
favorite author describes his settings, and apply that to your setting. 

3) If the room has a ceiling, walls and a floor, you must have descriptions for
all of those: Even if it's just something as simple as "The ceiling is high
above your head." If it's in the room, no matter how trivial it may seem, it
needs a description.
MAY
    );
    set_exits( ([
	"area2" : R + "app_part2",
	"area4" : R + "app_part4",
      ]) );
    set_objects( ([
      ]) );
}
