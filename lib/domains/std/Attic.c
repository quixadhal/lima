/* Do not remove the headers from this file! see /USAGE for more info. */

inherit INDOOR_ROOM;


void setup()
{
    set_brief("Attic");
    set_long( "A large chest glows softly as it sits against a wall of this small, dusty attic. Set into the wall above the chest is a brass dish, and within this, a blue flame burns. A flight of stairs plunge steeply down to the Grand Hall below." );
    set_objects( ([
	STAIRS : ({ "", "Wizroom" }),
	"dish" : 1,
	"chest" : 1,
	"web" : 1,
	"magic_torch" : 1,
      ]) );
}
