/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ROOM;

void setup() {
    set_brief("Beek's Workroom");
    set_long("This room is far too cool to need a description.");
    set_light(1);
    set_objects( ([
	"beek" : 1
    ]) );
    set_exits( ([
	"house" : "inside_house"
    ]) );
}
