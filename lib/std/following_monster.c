/* Do not remove the headers from this file! see /USAGE for more info. */

/* 
 * A wandering monster object.
 * 042998 Created by Tigran
 */

inherit MOVING_MONSTER;
inherit M_FOLLOW;

void mudlib_setup()
{
  moving_monster::mudlib_setup();
  m_follow::mudlib_setup();
}
