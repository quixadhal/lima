/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_READABLE;

void create()
{
  ::create();
  set_id ("wall","inscription", "writing", "passage");
  set_in_room_desc ("There is an inscription carved into the south wall.");
  set_long ("The inscription is carved into the cave wall.  You notice under the inscription the outline of a passage.\n");
  set_text ("It says:\n\tThis space intentionally left blank.\n");
  set_size (TOO_LARGE);
}
