/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OUTDOOR_ROOM;

void setup(string n, string e, string s, string w) {
    string array d = ({});
    string i;

    if (n) {
	add_exit("north", __DIR__ + "town/" + n);
	d += ({ "north" });
    }
    if (s) {
	add_exit("south", __DIR__ + "town/" + s);
	d += ({ "south" });
    }
    if (e) {
	add_exit("east", __DIR__ + "town/" + e);
	d += ({ "east" });
    }
    if (w) {
	add_exit("west", __DIR__ + "town/" + w);
	d += ({ "west" });
    }
    switch (i) {
    case 1:
	set_brief("Dead end");
	set_long("The road dead-ends here.  It continues to the " + d[0] + ".\n");
	break;
    case 2:
	set_brief("Town road");
	set_long("The road leads " + d[0] + " and " + d[1] + ".\n");
	break;
    default:
	set_brief("Intersection");
	set_long("The road leads " + format_list(d) + ".\n");
	break;
    }
}
