/* Do not remove the headers from this file! see /USAGE for more info. */

// By Ohara

inherit HIDDEN_DOOR;


string extra_long_stuff()
{
    if( query_closed())
	return "";
    return capitalize( the_short()) + " is open.\n";
}


mixed door_check()
{
  string ret = environment( this_object())->query_default_error();
  if( query_closed())
	  return ret;
  return 1;
}
