#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to make a simple shopkeeper");
    set_long(@MAY

Start with a file similar to simple npc - but inheriting M_VENDOR as well.

inherit ADVERSARY;
inherit M_VENDOR;

Within the setup, specify what he sells by set_sell( ([ ]) ).
First element of each line= filename of object to sell
Second element = number in stock (-1 for limitless supply)

    set_sell(([
     __DIR__ "apple" : -1,
     OBJS "sword" : 3,
     ]));

}
MAY
    );
    set_exits( ([
	"zoo" : R "zoo",
	"forward" : R "shopkeeper02",
      ]) );
    set_objects( ([
      ]) );
}
