#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to make a monster wear and wield");
    set_long(@MAY
inherit ADVERSARY;
void setup() {
    ::setup();
    set_name("Monster");
    set_id("monster");
    set_gender(1);
    set_proper_name("Frank the Monster");
    set_in_room_desc("Frank the Monster");
    set_long(@KYLISSA
  This is a very scary monster.
KYLISSA
    );
    set_wielding("/wiz/kylissa/gotham/W/skull_sword");
    set_wearing("/wiz/kylissa/gotham/A/skull_plate");
}
MAY
    );
    set_exits( ([
	"monsters" : R + "zoo",
      ]) );
    set_objects( ([
      ]) );
}
