#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Includes -- part two");
  set_long(@MAY
I have defined MAIN to always mean /domains/school
therefore R means /domains/school/R
because R includes the define MAIN as well.
The major advantage of this system is if I move this from one directory to
another I merely have to redefine MAIN and everything will work.

There is a second step.  How to put this into a file.
#include "school.h"  
as the top line in the code will do just that.  Then to use it in exits
you simply do the following:
add_exit( ([
     "direction" : R + "filename",
]) );
As you can see R is the "shortcut" from the .h file.


MAY
  );
  set_exits( ([
    "rooms" : R + "room",
    "backward" : R + "includes01",
   ]) );
  set_objects( ([
   ]) );
}
