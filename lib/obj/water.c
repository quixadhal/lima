/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <setbit.h>

inherit OBJ;
inherit M_GETTABLE;

create() {
  ::create();
  set_proper_name("some water");
  set_in_room_desc("There is a puddle of water here.");
  set_flag(NO_AUTO_TAKE);
  set_flag(DRINKABLE);
  set_weight(4);
  set_adjectives( ({ "quantity of", "pool of", "puddle of" }) );
  set_id( ({ "water", "liquid", "h2o" }) );
  set_long("You see nothing special about the water.\n");
  set_gettable("Try putting it in something.\n");
  set_dropable("Try pouring it into or on something.\n");
}

int move_water(object target) {
    if (present("water", target)) {
        write(capitalize(target->the_short())+" is already full of water.\n");
        return 0;
    }
    move(target);
    return 1;
}
