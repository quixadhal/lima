#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to make a simple object");
    set_long(@LORIEL
First inherit the underlying object:

inherit OBJ;

Then, in the setup, set the 3 basic descriptions needed:
id is the name that you use to interact (eg look at it by 'look map')
in_room_desc is what you see at the bottom of the room desc
long is what you see when you look at the map

void setup() {
  set_id("map");
  set_in_room_desc( "A map of Lima Bean has been discarded here.");
  set_long("It has a crude map of Lima Bean written on it.");
}
LORIEL
    );
    set_exits( ([
	"corridor" : R + "corridor05",
	"east" : R + "object02",
      ]) );
    set_objects( ([
  "/domains/std/map" : 1,
      ]) );
}
