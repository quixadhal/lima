/* Do not remove the headers from this file! see /USAGE for more info. */

// Outdoor rooms are naturally light, and may eventually suffer from
// weather.
inherit OUTDOOR_ROOM;

void setup()
{
    // The exit message for trying to enter the water.
    // The # at the beginning indicates a string to be printed rather
    // than a destination.
    string o = "#You would drown!\n";

    // The room's title.
    set_brief("Sandy beach");

    // The area this room is in.
    set_area ("pirate");

    // The room's description (not seen when brief is on)
    set_long("The waves lap happily onto this sandy beach, which ends here -- the mountains barely meet the water, leaving not even a grain of sand on which to walk."); 

    // The message given when the player tries to walk in any direction
    // but an explicitly defined direction.
    set_default_exit( "The mountains are too steep to climb.  You'd have to go around them...\n");

    // Places to go - in this case there are no places: All the exits
    // are strings to be printed when the player tries to use them.
    set_exits( ([
	"south" : o,
	"southeast" : o,
	"southwest" : o,
      ]) );

    set_exit_msg("(wade)", ({
	// Messages given when someone wades out. The "(wade)" is the name
	// passed.
	"$N $vwade into the ocean.  Though the undercurrent "
	"is strong, $n $vmanage to keep $p balance.  $N $vwander "
	"towards the east, where there is no beach, but there are "
	"interesting mountains...",

	"$N $vwade around in the ocean for a while, "
	"eventually wandering out of sight to the east."
      }) );

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
	write("(wade in the water)\n");
    case "water":
    case "waves":
    case "surf":
    case "ocean":
	// The "(wade)" is the exit name passed, which is used by set_exit_msg()
	this_body()->move_to(__DIR__ "Outside_Cave", "(wade)");
	QUEST_D->grant_points(this_body(),"pirate:foundCave");
	return 1;

    default:
	return "How stupid!\n";
    }
}
