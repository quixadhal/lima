/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OUTDOOR_ROOM;

void setup()
{
    string o = "#You would drown!\n";
    set_brief("Sandy beach");
    set_area ("pirate");
    set_long("The waves lap happily onto this sandy beach, which ends here -- the mountains barely meet the water, leaving not even a grain of sand on which to walk."); 

    set_default_exit( "The mountains are too steep to climb.  You'd have to go around them...\n");
    set_exits( ([
	"south" : o,
	"southeast" : o,
	"southwest" : o,
      ]) );


    add_item( "tide", "You can't look at it, but if you were to wade in the water, you sure would feel it." );

    add_item("mountains", 
      ([
	"adjs" : "steep", // Can also use an array...
	"look" : "They are too steep to climb.",
	"climb" : "They are too steep to climb." 
      ]));

    add_item("hills", "They're not hills, they're mountains!");

    set_hidden_exits( "all");

    set_objects( ([
	"/domains/std/objects/sand_castle" : 1,
	"/domains/std/objects/sand_with_treasure" : 1,
	"/domains/std/objects/welcome_sign" : 1,
	"/domains/std/objects/ocean" : 1,

      ]) );
}

mixed wade(string str)
{
    switch(str)
    {
    case 0:
	write("[in the water]\n");
    case "water":
    case "waves":
    case "surf":
    case "ocean":
	write ( "You wade into the ocean.  Though the undercurrent "
	  "is strong, you manage to keep your balance.  You wander "
	  "towards the east, where there is no beach, but there are "
	  "interesting mountains...\n\n");
	this_body()->other_action("$N $vwade around in the ocean for a while, "
	  "eventually wandering out of sight to the "
	  "east.\n");
	this_body()->move(__DIR__ "outside_cave");
	QUEST_D->grant_points(this_body(),"pirate:foundCave");
	this_body()->force_look();
	this_body()->other_action("$N $vwade over to you.\n");
	return 1;
    default:
	return "How stupid!\n";
    }
}

