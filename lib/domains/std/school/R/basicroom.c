// Yourwizname   Date   (example:  May  02 Feb 2001)

#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Skeleton Room Requirements");
    set_long(@MAY
#include "yourareaname.h";
inherit ROOM;
void setup(){
    set_light(1);
    set_area("schoolroom");
    set_brief("Skeleton Room");
    set_long(@YOURWIZNAME
This is your basic room (skeleton room).
YOURWIZNAME
    );
    add_item("desk",  
      ([ "look" :(@YOURWIZNAME
Item descriptions go here
YOURWIZNAME),             
      ]) );
    set_exits( ([
        "may" : R + "workroom",
      ]) );
    set_objects( (["/wiz/kaan/obj/kaan_throne" : 1,
//or set_objects( ([O + "filename" : 1,
      ]) );
}
MAY
    );
    set_exits( ([
	"rooms" : R "room",
      ]) );
    set_objects( ([
      ]) );
}
