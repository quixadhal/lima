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
    "edtutor" : R "edtutor01",
    "backward" : R "school",
    "forward" : R "corridor02",
      ]) );
    set_objects( ([
      ]) );
}
