#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Corridor");
    set_long(@LORIEL
This corridor leads new wizards onwards from the school,
gaining knowledge as they proceed.
LORIEL
    );
    set_exits( ([
    "monsters" : R "zoo",
    "backward" : R "corridor07",
    "forward" : R "corridor09",
      ]) );
    set_objects( ([
      ]) );
}
