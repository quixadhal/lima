/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_DIGGABLE;
inherit M_INPUT;

void setup()
{
  set_adj("sand", "intricate", "towering");
  set_id("castle","sculpture");
  set_in_room_desc("An intricate sand castle towers before you.");
  set_long("The sand castle is obviously the work of an artistic genius. Its towers and walls are carved in loving detail.  If you weren't on a beach, you might not even guess that this sculpture is made of sand.  You poke at it a bit, and it doesn't budge... It might as well have been made of stone!");
  set_size(TOO_LARGE);
}

void do_enter()
{
  write("You can't seem to find a way in, there are no doors.\n");
}


mixed direct_enter_obj()
{
  return "You can't seem to find a way in, there are no doors.\n";
}

void dig(object o)
{
  this_body()->simple_action("$N $vtry to dig up the sand castle, but $vfind "
			      "that it is as hard as a rock.");
}
