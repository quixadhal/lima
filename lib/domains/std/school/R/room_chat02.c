#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to add choice of room chat");
    set_long(@MAY

A choice of room chats can be give, by putting them in a string array:

  set_room_chat( ({
    "This is one of the options",
    "This is a different choice",
    "Yet another option",
    }),
    5);

MAY
    );

  set_room_chat( ({
    "This is one of the options",
    "This is a different choice",
    "Yet another option",
    }),
    5);

    set_exits( ([
	"backward" : R "room_chat01",
	"forward" : R "room_chat03",
      ]) );
    set_objects( ([
      ]) );
}
