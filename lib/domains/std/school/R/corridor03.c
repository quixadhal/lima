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
    "areas" : R "app_part1",
    "backward" : R "corridor02",
    "forward" : R "corridor04",
      ]) );
    set_objects( ([
      ]) );
}
