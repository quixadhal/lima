#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("New Wizard Information -- part 1");
    set_long(@MAY

New wizards need to know a few basic commands and how to get started.

First type wiz /on    -- this turns on the wiz line.  type wiz hello

Next type set PROMPT %p >
This after you type cd  will show /wiz/yourname >

MAY
    );
    set_exits( ([
	"down" : R + "school",
	"north" : R + "newwiz2",
      ]) );
    set_objects( ([
      ]) );
}
