#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to make a basic amulet");
    set_long(@MAY
#include <bodyslots.h>
inherit ARMOR;

void setup() {
    set_adj("Dusty","dusty","neck");
    set_size(3);
    set_armor_class (4);
    set_id("Desert Amulet","Amulet","amulet","desert amulet","armor","armour");
    set_long(@KRAMER
This is an amulet of the Mythryl Desert Valley. It seems to have some sort of
magical aura because it is glowing a little and is making somewhat of a faint
noise.
KRAMER
    );
    set_size(SMALL);
    set_slot("neck");
}
MAY
    );
    set_exits( ([
	"armory" : R "armory",
      ]) );
    set_objects( ([
      ]) );
}
