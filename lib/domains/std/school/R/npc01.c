#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to make a simple npc");
    set_long(@MAY
Start with a file similar to simple object - but inheriting ADVERSARY.
Add various names and descriptions - id, in_room_desc and long.

inherit ADVERSARY;
void setup() {
    set_id("dragon", "tiamat");
    set_in_room_desc("Tiamat the Dragon");
    set_long(@LIMA
Getting close enough to see what a dragon looks like
is not a good idea.
LIMA);

}
MAY
    );
    set_exits( ([
	"monsters" : R "zoo",
	"forward" : R "npc02",
      ]) );
    set_objects( ([
      ]) );
}
