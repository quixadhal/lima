/* Do not remove the headers from this file! see /USAGE for more info. */

// This object can function in a remarkably similar way to /domains/std/horse
// Save for the fact that you can't mount or dismount a bed, and beds don't
// normally move =)

inherit FURNITURE;

void create()
{
  ::create();
  set_id("bed");
  set_in_room_desc("There is a bed in the corner of the room.");
  set_get_on_msg("$N $vsit on the bed.\n");
  set_get_off_msg("$N $vget off the bed.\n");
  set_long("It's a bed...");
  set_max_capacity(VERY_LARGE*3);
  set_preposition("on");
}

// Unlike most furniture, you can lie on a bed.  We'll assume you mean sit
// on the bed here, but you can do what you want...

int direct_lie_on_obj()
{
  return 1;
}

int lie()
{
  return sit();
}
