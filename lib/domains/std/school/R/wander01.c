#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Wandering npc");
    set_long(@MAY

Adapt from the example used in the basic or simple npc.

Start by inheriting WANDERING_MONSTER instead of ADVERSARY
inherit WANDERING_MONSTER;

Within setup, handle the actual wandering:
Set the area where it will wander (see also next room)
    set_wander_area("wiz_area");

Set time between movements
    set_wander_time(5);

Set number of moves before it stops wandering.
If a player is met, the count restarts.
    set_max_moves(10);

Start wandering
    call_out("start_wandering",0);

MAY );

    set_area("wiz_area");

    set_exits( ([
	"backward" : R "zoo",
	"forward" : R "wander02",
      ]) );

         set_objects( ([
      ]) );
}
