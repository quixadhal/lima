#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("New Wizard Information -- part 2");
    set_long(@MAY

home  takes you to your workroom
home may   would take you to May's workroom

goto  takes you to a place or a person
goto may   would take you to May wherever she is

trans may   transports may to you
(ask first, she may not wish to come visiting)

MAY
    );
    set_exits( ([
	"south" : R + "newwiz1",
	"north" : R + "newwiz3",
      ]) );
    set_objects( ([
      ]) );
}
