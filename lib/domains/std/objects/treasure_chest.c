/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CONTAINER;
inherit M_OPENABLE;
inherit M_READABLE;
inherit M_GETTABLE;

void setup() {
    
    set_adj("large", "treasure", "sandy");
    set_id("chest");
    set_untouched_desc("There is a large treasure chest at the bottom of the "
		       "hole.");
    set_long("The treasure chest is still a bit sandy, but otherwise has weathered "
	     "well.  There is some writing on the outside of it.\n");
    set_gettable(1);
    set_objects( ([
		   ]) );
    set_size(LARGE);
    set_max_capacity(LARGE);
    set_closed(1);
    set_text("Written on the treasure chest is a poem:\n"
	     "\tI used to have a treasure chest.\n"
	     "\tIt got so heavy that I had to rest.\n"
	     "\tI let it slip away from me,\n"
	     "\tBut I didn't need it anyway,\n"
	     "\tSo I let it slip away...\n");
}

	    
