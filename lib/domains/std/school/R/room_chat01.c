#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to add simple room chat");
    set_long(@MAY

Room chat is periodic message to the room.

In its simplest version it is just a string for the message,
and an integer (number) for the period.

  set_room_chat("This is a simple room chat", 5);
MAY
    );

  set_room_chat("This is a simple room chat", 5);

    set_exits( ([
	"backward" : R "room",
	"forward" : R "room_chat02",
      ]) );
    set_objects( ([
      ]) );
}
