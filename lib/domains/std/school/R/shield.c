#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to make a basic shield");
    set_long(@MAY
#include <bodyslots.h>

inherit ARMOR;

void setup() {
    set_adj("Black","black","right","arm");
    add_adj("right");
    set_size(39);
    set_armor_class (10);
    set_id("shield","Shield","armor","armour");
    set_long(@KAAN
This is a black shield for demonstration.
KAAN
    );
    set_size(LARGE);
    set_slot("right hand");
}
MAY
    );
    set_exits( ([
	"armory" : R "armory",
      ]) );
    set_objects( ([
      ]) );
}
