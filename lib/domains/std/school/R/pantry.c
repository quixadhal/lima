#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("Wizard's Pantry [home of the foodstuffs]");
  set_long(@MAY
Among the myriad shelves here in the pantry can be found all types of
foodstuffs.  Untold time has been spent in here in preparations of many
good times and a few fiascos.  Explore though these rooms and learn.
MAY
  );

  add_item("shelves",  
   ([ "look" :(@MAY
These huge brown shelves are carefully bolted to the wall.
MAY),             
   ]) );


  add_item("foodstuffs",  
   ([ "look" :(@MAY
These are the various foods and drinks kept here.
MAY),             
   ]) );

  set_exits( ([
    "corridor" : R "corridor07",
    "apple" : R + "apple",
    "ale" : R + "ale",
   ]) );
  set_objects( ([
   ]) );
}
