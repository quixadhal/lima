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

    mixed swim( string verb )
{

    if( present("wall", environment(this_object()))->query_closed())
    this_body()->simple_action( "$N $v" + verb + " around the water for a while.");
else
  this_body()->simple_action("$N $v" + verb + " around in the water a bit."
			     "The cave floor quivers a bit as $n "
			     "$vclamber out.");
  call_out((: present("wall", environment(this_object()))->open_with( ) :), 1);
call_out((: present("wall", environment(this_object()))->close() :), 8);
  return 1;
}



