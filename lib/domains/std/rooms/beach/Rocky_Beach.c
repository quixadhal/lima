/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OUTDOOR_ROOM;


void
setup()
{
    string m = "#The mountains are too steep to climb.\n";
    string o = "#You would drown!\n";
  set_brief("Rocky beach");
  set_area ("pirate");
  set_long("There isn't much beach here to speak of; instead, there is a lot of rubble and debris.  The mountains and ocean surround you, although you can squeak back out through the surf.");
  set_exits( ([
	       "west" : m,
	       "northeast" :  m,
	       "northwest" :  m,
	       "north" : m,
	       "east" : m,
	       "south" : o,
	       "southeast" : o,
	       "southwest" : o,
	       ]) );

  add_item("mountains", "steep mountains", 
	   ([
	     "look" : "They are too steep to climb.",
	     "climb" : "They are too steep to climb." 
	     ]));

  add_item("hills", "They're not hills, they're mountains!");

  add_item( "sand", "beach" , "There's not much of that here, just "
	   "a lot of rocks.");
	       
  set_hidden_exits("northeast","northwest","south","southeast","southwest", 
		   "north", "east", "west");
  set_objects( ([
		 "/domains/std/objects/ocean" : 1,
		 "/domains/std/objects/debris" : 1,
		 ]));
}


mixed wade(string str)
{
  if(!str)
    {
      str = "water";
      write("[in the water]\n");
    }
  switch(str)
    {
    case "water":
    case "waves":
    case "surf":
    case "ocean":
      write ("You wade into the ocean, and back to the cave entrance.\n\n"); 
      this_body()->other_action("$N $vwade away.\n");
      this_body()->move(__DIR__ "Outside_Cave");
      this_body()->do_game_command("look");
      this_body()->other_action("$N $vwade over to you.\n");
      return 1;
    default:
      return "How stupid!\n";
    }
}

