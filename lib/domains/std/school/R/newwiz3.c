#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("New Wizard Information -- part 2");
    set_long(@MAY

ed will allow you to edit a file

update will attempt to compile and load the file

clone will produce a copy of the object in the file

dest will destruct an item

MAY
);
    set_exits( ([
	"south" : R "newwiz2",
	"academy" : R "school",
      ]) );
    set_objects( ([
      ]) );
}
