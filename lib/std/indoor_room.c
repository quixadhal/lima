/* Do not remove the headers from this file! see /USAGE for more info. */

inherit BASE_ROOM;

int is_indoors ()
{
  return 1;
}

void create ()
{
  ::create ();
  add_id_no_plural ("ground");
  add_id ("room");
}

