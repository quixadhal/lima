#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Includes -- part one");
  set_long(@MAY
A .h or header file is nothing more than a group of define that are
placed in a single file to be used by .c files.

The defines are simply a shorthand way of writing longer paths in
a simplified way.  for example:

#define MAIN     "/domains/std/school/"
#define R        MAIN + "R/"
#define X        MAIN + "X/"
#define A        MAIN + "A/"
#define W        MAIN + "W/"
#define M        MAIN + "M/"
#define F        MAIN + "F/"
#define O        MAIN + "O/"


MAY
  );
  set_exits( ([
    "rooms" : R + "room",
    "forward" : R + "includes02",
   ]) );
  set_objects( ([
   ]) );
}
