// May's Ed Tutorial for Wizards

#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Ed Tutorial 4");
  set_long(@MAY
Once you have made a file, you want to save it.  There
are many ways to save and or quit in ED.  The easiest way to both
save and quit is by typing <x>.  The only thing is that you must
be editing a file that already exists.  If you just typed ED
when you started, ED will not know the name to save your
file under so it will not allow you to quit.
   There are also times when you will not want to save when
you quit, for instance when you make a mistake and you want
to get back to the origional file.  In this case you can
type <Q> or <q>.  The capital one will exit you from Ed
no matter what and the lower case one will exit you
only if the file has not been changed since you opened it
or saved last.
MAY
  );
  set_exits( ([
    "south" : R + "edtutor03",
    "north" : R + "edtutor05",
   ]) );
  set_objects( ([
   ]) );
}
