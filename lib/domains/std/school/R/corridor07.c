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
    "pantry" : R "pantry",
    "backward" : R "corridor06",
    "forward" : R "corridor08",
      ]) );
    set_objects( ([
      ]) );
}
