#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("Area Construction -- part 1");
    set_long(@MAY
Subject: requirements for area construction

Lords of the domains are responsible for maintaining standards.

The following rooms outline generally accepted standards.

MAY
    );
    set_exits( ([
	"corridor" : R + "corridor03",
	"area2" : R + "app_part2",
      ]) );
    set_objects( ([
      ]) );
}
