/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_GETTABLE;
inherit M_WRITING_SURFACE;

void create() {
    ::create();
	
    set_adj("piece of", "scrap of");
    set_id("paper", "scrap");
    set_long("It's a rather plain scrap of white paper.\n");
    set_gettable(1);
    set_space(100);
}
     
