/* Do not remove the headers from this file! see /USAGE for more info. */

inherit FURNITURE;

void create()
{
  ::create();
  set_id("bed");
  set_in_room_desc("There is a bed in the corner of the room.");
  set_long("It's a bed...");
  set_max_capacity(VERY_LARGE+LARGE);
}
