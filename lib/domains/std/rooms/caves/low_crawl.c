/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <size.h>
inherit ROOM;

void setup()
{
    set_area("pirate");
    set_brief("Low Crawl");
    set_long(
      "You have to get on your hands and knees to proceed through this crawl.  Even on your hands and knees your back is scraping the ceiling causing loose rocks and gravel to fall around you making it difficult to move.  To the north the crawl widens, and from the south you feel a breeze of air that smells faintly salty strike your face.\n");
    set_light(0);
    set_exits( ([
      "south" : "/domains/std/rooms/caves/grotto.c",
   "north" : "/domains/std/rooms/caves/inside_cave.c",
    ]) );
    set_default_exit( "There are tales of mighty wizards who walk through walls, but unfortunately\nthese are but tales without foundation.  Exits to the south, and\nnorth may prove more fruitful.\n");

    add_item( "crawl", "low crawl" , "As you try and look around, more rocks and gravel fall from the ceiling and you quickly shut your eyes.");

    add_item( "ceiling" , "There is no way you can turn your head around that far to see above you." );

    add_item( "rock", "rocks", "gravel", "rocks and gravel" , 
	      ([ "look" : "The rocks and gravel have accumulated on the "
                	       "floor of the crawl making it very uneven.",
		 "get" : "They are too small to be useful." 
	       ]));

    set_max_capacity(VERY_LARGE+3*MEDIUM); // A person + 3 medium items
}



