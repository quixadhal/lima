#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to make a basic sword");
    set_long(@MAY

Inherit the appropriate file - SWORD, M_VALUABLE.
Normal setup for adj, size and value (id not needed).
Set weapon_class - approx max hps of damage from a hit.

inherit SWORD;
inherit M_VALUABLE;

void setup() {
    set_adj("dull");
    set_weapon_class(15);
    set_size(MEDIUM);
    set_value(1000);
}

MAY
    );
    set_exits( ([
        "weaponry" : R + "weaponry",
        "forward" : R "sword02",
      ]) );
    set_objects( ([
      "/domains/std/objects/sword" : 1,
      ]) );
}
