#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Extending a simple object");
    set_long(@LORIEL
First inherit additional modules:
To make it 'gettable' add M_GETTABLE, for reading add M_READABLE

inherit M_GETTABLE;
inherit M_READABLE;

More can now be set in the setup function -
untouched_desc for how it shows in the room before being touched
text for what you see when you read it

    set_untouched_desc("A map of Lima Bean is pinned to the wall.");
    set_text(@FOO
             MORTAL AREA   ATTIC
                  |       /
                  |      /  QUIET ROOM
    LAVA CAVE     |     /  /
             \    |    /  /       PLAINS
              \   |  U/  /           |
  ELEVATOR -- WIZARD HALL ---- EXAMPLE ROOM -- CAR WASH
             /D   |                  |
            /     |                  |
GENERAL STORE     |          RELATIONS & TOYS  -- ENVIRONMENTAL ROOM
               MONSTER ROOM          |
                                     |
                               CULT OF LIMA
FOO);
LORIEL
    );
    set_exits( ([
	"west" : R + "object01",
	"east" : R + "object03",
      ]) );
    set_objects( ([
  "/domains/std/map" : 1,
      ]) );
}
