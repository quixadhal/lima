/* Do not remove the headers from this file! see /USAGE for more info. */

// By Ohara

inherit HIDDEN_DOOR;


string extra_long_stuff()
{
    if( query_closed())
	return "";
    return capitalize( the_short()) + " is open.\n";
}
