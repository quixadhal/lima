#include "../school.h";
inherit ROOM;

void setup(){
    set_brief("How to make platemail");
    set_long(@MAY
#include <bodyslots.h>
inherit ARMOR;

void setup() {
    set_adj("Black","black","torso");
    set_size(73);
    set_armor_class (8);
    set_id("platemail","Platemail","mail","plate","armor","armour");
    set_long(@KAAN
A black platemail that is made out of some strange substance. It has
no refelctive surfaces but shapes in the form of a black storm trooper's 
torso shape.
KAAN
    );
    set_size(LARGE);
    set_slot("torso");
}
MAY
    );
    set_exits( ([
	"armory" : R "armory",
      ]) );
    set_objects( ([
      ]) );
}
