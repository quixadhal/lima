/* Do not remove the headers from this file! see /USAGE for more info. */


inherit INF_SUPPLY;

void setup() {
    set_id("box");
    set_adj("magic");
    set_long("The magic box never runs out!");
    set_max_capacity(VERY_LARGE);
    set_objects( ([
	"/domains/std/objects/platemail" : -1,
	"/domains/std/objects/sword" : -1
      ]) );
}
