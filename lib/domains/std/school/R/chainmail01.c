#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to make basic armor");
    set_long(@MAY

Start as with simple object, but inherit the file ARMOR instead of OBJ.

Within the setup, two additional items to add:
set_armor_class(<num>) - damage absorbed will be random 1 to <num>
set_slot(<place>) - sets where it is worn.
Note slots are predetermined, and have size limits.

inherit ARMOR;

void setup() {
    set_adj("chainmail");
    set_id("shirt", "chainmail");
    set_armor_class(4);
    set_slot("torso");
}

MAY
    );
    set_exits( ([
	"armory" : R "armory",
	"forward" : "chainmail02",
      ]) );
    set_objects( ([
    "/domains/std/chainmail" : 1,
      ]) );
}
