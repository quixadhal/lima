// May's Ed Tutorial for Wizards

#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Ed Tutorial 6");
  set_long(@MAY
The add and insert commands <a> and <i> are quite
similar.  They both put you in to the 'add text' mode.
The difference between the 2 commands is where the new
text will be placed.  When using the <a> command, the new
lines are put AFTER the specified line.  If no line # is
given, then the text is put after the CURRENT line.
The <i> command works similarly but the text is INSERTED
BEFORE the specified line.  The same holds true if no
line number is given.
   To get out of the 'add text' mode, type a <.> and
Ed will put you back in the reqular editing mode.
   Now, when a file is opened for the first time and no
texts exists in the file, just type <a> and start to
add to the file.  Remember to type <w file_name.c> to
save it when you are ready.  Then type <q> to get out
of ED.  Re-enter ED with <ed file_name.c> and you
are ready to edit your file.
MAY
  );
  set_exits( ([
    "south" : R + "edtutor05a",
    "north" : R + "edtutor07",
   ]) );
  set_objects( ([
   ]) );
}
