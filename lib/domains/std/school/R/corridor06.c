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
    "weaponry" : R "weaponry",
    "armory" : R "armory",
    "backward" : R "corridor05",
    "forward" : R "corridor07",
    "emoting" : R "emote01",
   ]) );
  set_objects( ([
   ]) );
}
