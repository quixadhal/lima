#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to do an add_item");
    set_long(@MAY

The add_item follows the set_long in the code.  One item for each IMPORTANT
noun in the set_long.

    add_item("desk",  
      ([ "look" :
         "It defies belief, it's actually a desk made of dark matter.  "
         "Surely, it takes a great mind to use it, yet alone work on it.  "
         "To even place anything on the surface would destroy it.  "
         "There is only one drawer made of the same material."
         "\n",
      ]) );
MAY
    );

    add_item("desk",  
      ([ "look" :
         "It defies belief, it's actually a desk made of dark matter.  "
         "Surely, it takes a great mind to use it, yet alone work on it.  "
         "To even place anything on the surface would destroy it.  "
         "There is only one drawer made of the same material."
         "\n",
      ]) );

    set_exits( ([
	"backward" : R "room",
	"forward" : R "add_item02",
      ]) );
    set_objects( ([
      ]) );
}
