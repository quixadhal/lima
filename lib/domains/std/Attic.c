/* Do not remove the headers from this file! see /USAGE for more info. */

inherit INDOOR_ROOM;


// Player can go down, so return 1.
mixed can_go_down(){ return 1; }

// We want the player to use the stairs, so we force them to climb down
// and then return 1, to signify that we've handled the action completely.
int do_go_down()
{
    this_body()->force_game_command( "climb down stairs");
    return 1;
}


void setup()
{
    set_brief("Attic");
    set_long( "A large chest glows softly as it sits against a wall of this small, dusty attic. Set into the wall above the chest is a brass dish, and within this, a blue flame burns. A flight of stairs plunge steeply down to the Grand Hall below." );
    set_objects( ([
	STAIRS : ({ 1, 0, "Wizroom", 1 }),
	"dish" : 1,
	"chest" : 1,
	"web" : 1,
	"magic_torch" : 1,
      ]) );
}
