#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to make basic boots");
    set_long(@MAY
#include <bodyslots.h>
inherit ARMOR;
inherit M_READABLE;

void setup() {
    set_adj("Black","black","feet");
    set_armor_class (3);
    set_id("boots","Boots","armor","armour");
    add_id("inscription");
    set_long(@KAAN
These are black boots of the darkest black leather you have ever seen. 
They are sturdy looking boots. On the boots is a inscription.
KAAN
    );
    set_text(@KAAN
The inscription reads 'I am boots of swift, wear me, love me for I protect
your feet everywhere you go.'
KAAN
    );
    set_size(SMALL);
    set_slot("feet");
}
MAY
    );
    set_exits( ([
	"armory" : R "armory",
      ]) );
}
