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
    "unix" : R "unix",
    "backward" : R "corridor01",
    "forward" : R "corridor03",
      ]) );
    set_objects( ([
      ]) );
}
