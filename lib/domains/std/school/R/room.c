#include "../school.h";
inherit INDOOR_ROOM;

void setup(){
  set_brief("Room building");
  set_long(
   "This room looks like a construction site, "
   "with workmen dashing hither and thither, "
   "building materials scattered over the floor."
   "\n");
  set_exits( ([
    "corridor" : R + "corridor04",
    "basic": R "basicroom",
    "set_long": R "set_long01",
    "items": R "add_item01",
    "set_exits": R "set_exit",
    "roomchat": R "room_chat01",
    "includes" : R + "includes01",
   ]) );
}
