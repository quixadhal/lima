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
    "rooms" : R "room",
    "backward" : R "corridor03",
    "forward" : R "corridor05",
      ]) );
    set_objects( ([
      ]) );
}
