#include "../school.h";
inherit ROOM;

void setup(){
  set_brief("How to make a drink");
  set_long(@MAY

This example is a liquid - ie it slips through finger unless in a container
Start as with any simple object, but inherit FLUID and M_DRINKABLE

Optional extras within setup() 

// Custom message as it evaporates after being spilled 
    set_evaporate_action( "The puddle of ale dwindles." );

// Custom message as you taste it it
    set_taste_action( ({"It's a little bit weak, but it'll do the job.",""}) );

Similar messages can be added for drink and last drink etc
MAY
  );
  set_exits( ([
    "pantry" : R + "pantry",
   ]) );
  set_objects( ([
    "/domains/std/ale" : 1,
   ]) );
}
