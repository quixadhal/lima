#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Extras for basic armor");
    set_long(@MAY

Starting with chainmail, as in previous room:

#include <bodyslots.h>, so you use abbreviations (eg TORSO for "torso")

Within setup(), add custom wear messages
Use set_resist() for strength adjustment against different damage types.

#include <bodyslots.h>

    set_resist("blade", 2); // a little better against blades
    set_wearmsg( "$N $vput on a $o." );
    set_slot(TORSO);
}

MAY
    );
    set_exits( ([
	"finish" : R "armory",
	"backward" : "chainmail02",
      ]) );
    set_objects( ([
    "/domains/std/chainmail" : 1,
      ]) );
}
