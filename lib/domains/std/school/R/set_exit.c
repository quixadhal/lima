#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("How to do a set_exit");
  set_long(@MAY
To do a set_exit think of it this way:
   "direction going to" : "path of the room going to",

so...
   "may" : "/wiz/may/workroom",
will take you in the direction "may"
and the workroom is the location doing to
please note do not add the .c to the path.

// exits need to be the full path if they are in a different directory
// exits that use an include as areas are required to do use this:
// "direction" : R "filename",
// where R is defined in the .h file as the path needed to find this
// filename
    set_exits( ([
        "may" : "/wiz/may/workroom",
        "east" : "shop",
        "south" : "fmagehall",
        "west" : "donation"
      ]) );
// omit the next line if you do not want hidden exits
    set_hidden_exits("west", "east");
MAY
  );
  set_exits( ([
    "rooms" : R + "room",
   ]) );
}
