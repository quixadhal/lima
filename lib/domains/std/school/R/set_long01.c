#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to do a set_long");
    set_long(@MAY
Here you type in the text that you want to say about the room.

This is a quick glance of what you would see when you enter the room.
The add_item will give a fuller description of the room.

Please do not tell me what is in the next room.
Tell me about textures, colours, materials.

Avoid YOU or YOUR in any description.

Requirement is a minimum of 3 full lines of normal screen width (80 chars).
MAY
    );
    set_exits( ([
	"backward" : R "room",
	"forward" : R "set_long02",
      ]) );
    set_objects( ([
      ]) );
}
