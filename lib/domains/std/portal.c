/* Do not remove the headers from this file! see /USAGE for more info. */

inherit PORTAL;

void setup ()
{
  set_in_room_desc ("Rust's test portal is here.");
  set_long ("It looks like a portal.");
  add_adj ("test","rust's");
  add_id ("portal");
  set_destination ("/domains/std/room3");
  set_look_in_desc ("You can vaguely make out some other room.");
}
