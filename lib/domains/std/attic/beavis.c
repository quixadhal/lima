/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit LIVING;
inherit M_ACTIONS;
inherit M_TRIGGERS;

void follow_the_script_dumbass()
{
  run_script("example");
}



void setup() {
    set_name("Beavis");
    set_gender(1);
    set_proper_name("Beavis");
    set_in_room_desc("Beavis mills about, periodically scratching his nads.");
    set_long("His body odor prevents you from getting too close.");
    add_script("example", ({
3, "slut butthead", 
4, "scream in pain!"
}));

}
