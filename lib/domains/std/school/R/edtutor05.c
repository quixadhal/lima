// May's Ed Tutorial for Wizards

#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Ed Tutorial 5");
  set_long(@MAY
   Lastly there is the <w> command.  This saves the file you
are working on but does not exit you from ED.  Again
if you are working on a file with no name, you must type
<w file_name.c> and then <q>.  Then re-enter the file with
<ed file_name.c>.  At this time you can just type <w> or
<x> and the computer will know what name to save your file
under.
MAY
  );
  set_exits( ([
    "south" : R + "edtutor04",
    "north" : R + "edtutor05a",
   ]) );
  set_objects( ([
   ]) );
}
