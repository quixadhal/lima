/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OUTDOOR_ROOM;

void obj_arrived(object ob) {
    if (ob->get_size() >= VERY_LARGE) 
      {
     if(!ob->is_living())
	tell_room(this_object(), capitalize(ob->the_short()) + " makes a "
		  "large splash as it hits the water.\n");
	return;
      }
    tell_room(this_object(), capitalize(ob->the_short()) + " sinks into the surf, and you quickly loose track of it in the violent crashing waves.\n");
    destruct(ob);
}

create()
{
    string m = "#The mountains are too steep to climb.\n";
    string o = "#You would drown!\n";

  ::create();
  set_brief("Outside cave");
  set_area ("pirate");
  set_long("You are wading knee-deep in the surf.  Except for what seems to be a tiny cave, the surf yields to the rather steep mountains fairly violently, and you have trouble keeping your balance.  The mountains retreat a bit, leaving some sand to walk on to the east and west.");
  set_exits( ([
	       "in" : "../caves/grotto.c",
	       "north" : "../caves/grotto.c",
	       "east" : "rocky_beach.c",
	       "west" : "sandy_beach.c",
	       "northeast" :  m,
	       "northwest" :  m,
	       "south" : o,
	       "southeast" : o,
	       "southwest" : o,
	       ]) );

  add_item( "surf", "water", 
	   ([
	     "adjs": "fierce",
	     "look": "The surf is fierce.  It is all you can do to stay on "
	             "your feet.",
	     "wade": "You are already doing that.",
	     "swim": "You would surely die.",
	     ]));

  add_item( "cave", "tiny cave", "You can't see much of the cave from here.");

  add_item( "ocean", ([
	   "look" :  "You strain to see the other side of the "
	       "ocean, but fail.  You do notice something, however you can't "
	       "quite make out what.  Perhaps a ship?",
	   "wade":   "You are already doing that.",
	   "swim":   "You would surely die.",
	     ]));

  add_item( "mountains", "steep mountains", 
	    ([
	      "climb" : "They are too steep to climb.",
	      "look" : "They are too steep to climb.",
	     ]));

  add_item( "hills", "They're not hills, they're mountains!");

  add_item( "sand", "beach" , "There's a lot of it to the east and west." );

  set_hidden_exits("northeast","northwest","south","southeast","southwest", 
		   "in");

  add_hook("object_arrived", (: obj_arrived :));
}


void do_enter(string s)
{
  if(s == "cave")
    this_body()->do_game_command("in");
  else
    write(this_body()->nonsense());
}


mixed direct_enter_obj()
{
  return 1;
}
