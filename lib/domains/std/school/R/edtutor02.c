// May's Ed Tutorial for Wizards

#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Ed Tutorial 2");
  set_long(@MAY
Here you will learn the most basic stuff.  For example, how
to get started editing a file.
   To start editing a file, just type ed file_name.c
This turns the editor on and also puts you in the specified file.
If you just want to start from scratch, you can just type ed.
Now, when the editor first comes on, line numbers will not
appear with your text.  The <n> command toggles the line numbers.
   Since this is a LINE editor, you can only work on 1 line at
a time.  By typing a period, it will show you your current line.
Hitting return will advance you to the next line and show you that one.
You can also see a single line by just typing the number of
the line you want to see.  This will place you on that line
and show it to you also.
   If at any time, you want to see a list of the commands in ED, just
type <h> while in Ed.
   Lastly, there is a function called <auto-indent>.  The capital
I is the command to auto-indent your code.  This is a must
if you want other wizards to read your code.  It makes it nice
and neat and readable.  Use it on all your code.

MAY
  );
  set_exits( ([
    "south" : R + "edtutor01",
    "north" : R + "edtutor03",
   ]) );
  set_objects( ([
   ]) );
}
