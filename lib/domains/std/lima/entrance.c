/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ROOM;

void setup() {
    set_brief("West of House");
    set_long("You are standing in an open field west of a white house, with an open front door.  At the southwestern edge of the field, an overgrown track sets off into the trees.\n");
    set_light(1);
    set_objects( ([
	"small_mailbox" : 1,
	"white_house" : 1
    ]) );
    set_exits( ([
	"east" : "inside_house"
    ]) );
    set_default_exit_message("A hollow voice says: Fool!\n");
}
