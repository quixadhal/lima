/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;
inherit M_OPENABLE;
inherit M_GETTABLE;

void setup() {
    set_gettable("It is securely anchored.\n");
    set_id("mailbox", "box");
    set_adj("small");
    set_in_room_desc("There is a small mailbox here.");
    set_closed(1);
    set_max_capacity(SMALL);
    set_size(MEDIUM);
    set_objects( ([
	"leaflet" : 1
    ]) );
}
