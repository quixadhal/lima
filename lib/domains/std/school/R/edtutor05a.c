// May's Ed Tutorial for Wizards

#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Ed Tutorial 5a");
  set_long(@MAY
Command mode in ED is quite like clicking on an icon in windows because you
want to do something.  In ed it has : as a prompt and means you are going to
tell it what you want it to do using a command.

For example if I typed this:- :4s/hid/hide
it would tell ED to substitute on line 4 hide for hid.

Insert mode in ED is quite like typing on an old fashioned typewriter where
every line has to be returned before starting on the next one.  Normally you
enter insert mode by using a 'a, i, or c' in command mode.  This will bring up
the numbers if you have them turned on, and will look like this.

Say I wanted to add a line after 4, I would type 4a  which would bring up the
5. This also effectively pushes the rest of the code down, so don't worry, it
won't overwrite your code below it.  Think of a as after, i as insert or 
before, and c as change.  The next trick is to exit from insert mode back to
command mode.  Simple on a new line type a dot . then at the : type I to
autoindent your code, and x to exit command mode.
MAY
  );
  set_exits( ([
    "south" : R + "edtutor05",
    "north" : R + "edtutor06",
   ]) );
  set_objects( ([
   ]) );
}
