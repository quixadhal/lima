// May's Ed Tutorial for Wizards

#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Ed Tutorial 7");
  set_long(@MAY
The <d> command is for delete.  If you just type <d> it will
delete the current line.  You can specify a line to delete by
typing <#d> where # is the line number you want to remove.
You can also delete a range of lines by typing <start_#,end_#d>
to delete lines start_# to end_#.
   Remember that if you want to delete 2 lines and they are not near
each other, do the higher number lines first so you don't change
the numbering order.  For example, if you want to delete line 5 and
line 10, <10d> and then <5d> will work.  <5d> followed by <10d>
will cause you to delete the wrong line when you attempt
to delete line 10 because 5 is already gone.
MAY
  );
  set_exits( ([
    "south" : R + "edtutor06",
    "north" : R + "edtutor08",
   ]) );
  set_objects( ([
   ]) );
}
