/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;

void create()
{
  ::create();
  set_adj("lazy");
  set_id("river","water");
  set_in_room_desc("");
  set_long(wrap(
"The river is moving rather slowly from east to west.  It isn't very deep;"
"you won't have any problems fording it if you want to leave north.\n"));
  set_size(TOO_LARGE);
}

mixed swim()
{

  this_body()->simple_action("$N $vswim around in the water a bit.\n"
			     "The cave floor quivers a bit as $n "
			     "$vclamber out.\n");
  call_out((: environment(this_object())->open_passage() :), 3);
  return 1;
}

mixed wade()
{
  this_body()->simple_action("$N $vwade around in the water a bit. \n"
			     "The cave floor quivers a bit as $n "
			     "$vclamber out.\n");
  call_out((: environment(this_object())->open_passage() :), 3);
  return 1;
}


