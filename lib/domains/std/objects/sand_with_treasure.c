/* Do not remove the headers from this file! see /USAGE for more info. */

#include <size.h>

inherit OBJ;
inherit M_DIGGABLE;
inherit M_INPUT;  // Required for M_DIGGABLE.

int number_times_dug = 0;
object	my_hole;

void setup()
{
  set_adj("sandy");
  set_id("beach","sand");
  set_attached(1);
  set_long("There's lots of it.\n");
#ifdef USE_SIZE
  set_size(TOO_LARGE);
#endif
#ifdef USE_MASS
  set_mass(TOO_LARGE);
#endif
}

void dig(object o)
{
  switch(number_times_dug++)
  {
    case 0:
      my_hole = new(HOLE);
      my_hole->move(environment(this_object()));
      this_body()->simple_action("$N $vseem to be digging a hole here...");
      my_hole->change_desc("There is a small hole in the sand.");
      break;
    case 1:
      my_hole->set_in_room_desc("There is a hole in the sand.");
    case 2:
      this_body()->simple_action("$N $vdig the hole a bit deeper.");
      break;
    case 3:
      my_hole->change_desc("There is a large hole in the sand.");
      my_hole->change_max_capacity(VERY_LARGE);
    case 4:
      this_body()->simple_action("$N $vdig the hole a bit deeper.");
      break;
    case 5:
      my_hole->change_desc("There is a very large hole in the sand.");
      my_hole->change_max_capacity(VERY_LARGE*2);
    case 6:
      this_body()->simple_action("$N $vdig the hole a bit deeper.");
      break;
    case 7:
      this_body()->simple_action("$N $vdig at the hole some more and $vuncover "
				 "a treasure chest!");
      QUEST_D->grant_points(this_body(), "pirate");
      break;
    case 8:
      this_body()->simple_action("$N $vdig the hole a bit deeper.");
      break;
    case 9:
    case 10:
      this_body()->simple_action("$N $vdig the hole a bit deeper.");
      break;
    default:
      this_body()->simple_action("$N $vdig at the hole, but $vmake no progress, "
				 "as the hole fills in\neverything removed "
				 "from it.");
      break;
				
  }
}
