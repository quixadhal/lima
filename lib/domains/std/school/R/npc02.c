#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("More on the simple npc");
    set_long(@MAY

Continuing with the previous example, add name and proper_name.
Specify gender.

Name normally used
    set_name("Tiamat");

Formal name if needed
    set_proper_name("Tiamat");

Use 1 for male, 2 for female, 0 for neuter.
    set_gender(1);

Set the hit points
    set_max_health(300);

MAY );
    set_exits( ([
	"finish" : R "zoo",
	"backward" : R "npc01",
      ]) );
    set_objects( ([
      ]) );
}
