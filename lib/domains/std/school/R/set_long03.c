#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Room set_long - new style");
  set_long(
    "The 'new style' is like this:\n\n"
    "set_long(\n"
    "\"Each line starts and ends with quotes. \"\n"
    "\"The player's client will handle the line-wrapping, \"\n"
    "\"so it's easiest to start each sentence or clause on a new line, \"\n"
    "\"then switching order is much easier, specially with ed.  \"\n"
    "\"Disadvantage is that it's easy to omit one of the quotes.\"\n"
    "\"\\n\"\);\n"
    );
    set_exits( ([
	"backward" : R "set_long02",
	"forward" : R "set_long04",
      ]) );
    set_objects( ([
      ]) );
}
