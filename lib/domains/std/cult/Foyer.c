/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** foyer.c -- the entry foyer to the Cult of Lima
**
** 970217, Deathblade: created
*/

inherit INDOOR_ROOM;

void setup()
{
    set_area("wiz_area");
    set_brief("Cult of Lima foyer");
    set_long("This is the entry foyer to the Cult of Lima. "
	     "A large archway, filled with a shimmering curtain of light, "
	     "leads back into the building, "
	     "where you hear faint echoes of maniacal laughter. "
	     "A desk sits to one side of the room.");
    set_exits( ([
	"north" : "../Relations_And_Toys.c",
	]) );
    set_objects( ([
	"portal" : 1,
	]) );
    add_item("desk", "The desk is very neat and clean, with two small stacks of papers on it.");
    add_item("papers", "stacks of papers", "stacks",
	     "stack of papers", "forms", "releases",
	     ([
		 "look" : "The papers seem to be application forms and releases of liability.",
		 "read" : "Your head begins to spin as you try to understand the legalese on the forms.",
		 ])
	);
}
