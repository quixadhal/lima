/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <setbit.h>

inherit FLUID;
inherit M_DRINKABLE;

void mudlib_setup()
{
  set_proper_name("some water");
  set_in_room_desc("There is a puddle of water here.");
  set_adj( "quantity of", "pool of", "puddle of" );
  set_id( "water", "liquid", "h2o" );
  set_long("You see nothing special about the water.");
  set_gettable("Try putting it in something.\n");
   // set_droppable("Try pouring it into or onto something.\n");
}

int move_water(object target) {
    if (present("water", target)) {
        write(capitalize(target->the_short())+" is already full of water.\n");
        return 0;
    }
    move(target);
    return 1;
}
