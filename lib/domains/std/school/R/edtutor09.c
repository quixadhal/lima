// May's Ed Tutorial for Wizards

#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Ed Tutorial 9");
  set_long(@MAY
The search command lets you look for the specified string
and it looks like </TEXT>.  This command will find the next
occurance of TEXT in your code.  Remember this command is case
sensitive like all the other commands in ED.  If you want
to see all the occurances of TEXT in your code, you can
find them by typing <g/TEXT/p> meaning globally find TEXT
and print it out on the screen.
MAY
  );
  set_exits( ([
    "south" : R + "edtutor08",
    "north" : R + "edtutor10",
   ]) );
  set_objects( ([
   ]) );
}
