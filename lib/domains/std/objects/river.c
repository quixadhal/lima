/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;

void setup()
{
  set_adj("lazy");
  set_id("river","water");
  set_long(
"The river is moving rather slowly from east to west.  It isn't very deep;"
"you won't have any problems fording it if you want to leave north.\n");
  set_size(TOO_LARGE);
set_flag(ATTACHED);
}

mixed swim()
{

  this_body()->simple_action("$N $vswim around in the water a bit.\n"
			     "The cave floor quivers a bit as $n "
			     "$vclamber out.");
  call_out((: environment(this_object())->open_passage() :), 3);
  return 1;
}

mixed wade()
{
  this_body()->simple_action("$N $vwade around in the water a bit. \n"
			     "The cave floor quivers a bit as $n "
			     "$vclamber out.");
  call_out((: environment(this_object())->open_passage() :), 3);
  return 1;
}


