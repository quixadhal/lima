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
    "objects" : R "object01",
    "backward" : R "corridor04",
    "forward" : R "corridor06",
      ]) );
    set_objects( ([
      ]) );
}
