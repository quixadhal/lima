#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Area Construction -- part 2");
    set_long(@MAY
There are two preferred styles of subdirectory structure I will accept:
A F R W M O    or armor   food    rooms    weapon   npc   obj
Please name your area appropriately.  area/  is not acceptable but
caves/  is IF you area is caves.

Reminder:  no YOU or YOUR in any form is acceptable in your descriptions.
You need 3 lines of 75 columns in your descriptions as well.

Domain Lords:  Rules apply to you as well.

All rooms will contain items and will be dense.  By dense I mean every
room will have something to do in them, not necessarily just hack and slay.
Of course, there is more to an area than just rooms.  Monsters, Armor,
Weapons, Objects also need to be done for an area to be complete.

MAY
    );
    set_exits( ([
	"area3" : R + "app_part3",
	"area1" : R + "app_part1",
      ]) );
    set_objects( ([
      ]) );
}
