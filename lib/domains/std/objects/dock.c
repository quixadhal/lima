/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;

void create()
{
  ::create();
  set_adj("rotting", "rotten", "wooden");
  set_id("dock");
  set_attached(1);
  set_long("The dock has become so rotten that it's really pretty useless.\n");
  set_size(VERY_LARGE);
}

