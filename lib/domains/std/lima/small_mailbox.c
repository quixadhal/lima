/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;
inherit M_OPENABLE;
inherit M_GETTABLE;

void setup() {
  add_relation("in");
  set_default_relation("in");
  set_gettable("It is securely anchored.\n");
  set_id("mailbox", "box");
  set_adj("small");
  set_in_room_desc("There is a small mailbox here.");
  set_max_capacity(SMALL);
#ifdef USE_SIZE
  set_size(MEDIUM);
#endif
#ifdef USE_MASS
  set_mass(MEDIUM);
#endif
  set_objects( ([
		 "leaflet" : 1
  ]) );
}
