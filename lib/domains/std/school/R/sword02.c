#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to add custom combat messages");
    set_long(@MAY

Continuing with the basic sword, use add_combat_message() within setup.
This takes 2 arguments :
type (a string, such as dam1 - dam10 for various levels of damage)
message, using same tokens as in souls, custom entry messages etc

    add_combat_message("miss", "$N $vtake a clumsy swipe at $t, but only $vsucceed in making a fool of $r.");

See the standard combat messages in MESSAGES_D (/daemons/messages_d)
for a variety of examples.

MAY
    );
    set_exits( ([
        "backward" : R "sword",
        "weaponry" : R "weaponry",
      ]) );
    set_objects( ([
      "/domains/std/objects/sword" : 1,
      ]) );
}
