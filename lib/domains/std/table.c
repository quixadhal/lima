/* Do not remove the headers from this file! see /USAGE for more info. */

inherit COMPLEX_CONTAINER;

void setup() {
	
    set_id("table");
    set_adj("large");
    set_long("It's a large table, with plently of room under it.");
    set_relations("on", "under");
    set_size(LARGE);
}
