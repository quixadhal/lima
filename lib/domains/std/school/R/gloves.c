#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to make basic gloves");
    set_long(@MAY
#include <bodyslots.h>
inherit ARMOR;
void set_slot(string);

void setup() {
    set_adj("Black","black","hands");
    set_size(13);
    set_armor_class (3);
    set_id("gloves","Gloves","mitts","armor","armour");
    set_long(@KAAN
These are black gloves for demonstration
KAAN
    );
    set_size(MEDIUM);
    set_slot("hands");
}
MAY
    );
    set_exits( ([
	"armory" : R "armory",
      ]) );
}
