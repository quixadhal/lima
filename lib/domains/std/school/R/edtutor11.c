// May's Ed Tutorial for Wizards

#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Ed Tutorial 11");
  set_long(@MAY
The MOVE and COPY commands are not used quite as
often as the 's' command, but they still are quite useful.
They either MOVE or COPY entire lines to some other place
within your code. The commands are 't' and 'm' for COPY
and MOVE respectively.  The line #'s you want to move or copy
come first followed by the 't' or 'm' and then the line #
you want the text to come AFTER comes last.  For instance,
if you wanted to MOVE lines 10 to 25 to after line 100
you would type <10,25 m 100>.  COPY works the same way
but you don't take the original lines out of the code
like you do when you use MOVE.
   You can also import in entire copies of files from outside your
code. This is done with the 'r' command.  It imports the file into
your current file where ever you designate.  If you don't
designate a line #, the imported file goes at the end of the
current file being edited.  The import command looks like:
<# r /path/name.c>.  You must specify the entire file path
and name of the file being imported.
MAY
  );
  set_exits( ([
    "south" : R + "edtutor10",
    "north" : R + "edtutor12",
   ]) );
  set_objects( ([
   ]) );
}
