/* Do not remove the headers from this file! see /USAGE for more info. */

/* 
 * A wandering monster object.
 * 042998 Created by Tigran
 */

inherit MOVING_MONSTER;
inherit M_WANDER;

void mudlib_setup()
{
  set_wander_time(10);
  set_max_moves(10);
  moving_monster::mudlib_setup();
}
