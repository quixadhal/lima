#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to add function as room chat");
    set_long(@MAY

A function can be used in a room chats, by putting it in (: :).
This example uses the function time() to return the current time,
then ctime() to convert it to a 'sensible' form.

  set_room_chat( (: ctime(time()):), 5);

MAY
    );

  set_room_chat( (: ctime(time()):), 5);

    set_exits( ([
	"backward" : R "room_chat02",
	"finish" : R "room",
      ]) );
    set_objects( ([
      ]) );
}
