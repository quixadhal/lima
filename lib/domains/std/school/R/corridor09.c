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
    "problems" : R "problems01",
    "backward" : R "corridor08",
    "finish" : R "school",
      ]) );
    set_objects( ([
      ]) );
}
