#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Wandering npc - part 2");
    set_long(@MAY

The wandering monster's wander zone can be specified.
If no wander zone is set, it will wander everywhere.
If one is set, it will only wander to rooms with corresponding zone.

In this example, this room and the previous room have been set thus:

  set_area("wiz_area");

MAY );

    set_area("wiz_area");

    set_exits( ([
	"finish" : R "zoo",
	"backward" : R "wander01",
      ]) );
    set_objects( ([
    "/domains/std/dragon" : 1,
      ]) );
}
