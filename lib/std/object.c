/* Do not remove the headers from this file! see /USAGE for more info. */

/* object.c  about 1/2 Rust and 1/2 Nevyn  */
// Modifications:  Peregrin@ZorkMUD (GUE),  Mar '94
// Beek - rewrote the DISCARDED_MESSAGE stuff b/c it was hideously
//      - inefficient.
// Beek --- Added the OFFICIAL "what these vars should contain"
// documentation
// Sep 9 1994
// Rust moved everything into modules.

#include <flags.h>

inherit "/std/object/mass";
inherit "/std/object/properties";
inherit "/std/object/description";
inherit "/std/object/move";
inherit "/std/object/non_object";
inherit "/std/object/visible";
inherit "/std/object/flags";
inherit "/std/object/vsupport";
inherit "/std/object/hooks";

//:FUNCTION stat_me
//write() some debugging info about the state of the object
int stat_me() {
    write("Short: "+short()+"\n");
    write("Weight: "+query_mass()+"  Light: " + query_light() + "\n");
    write("IDs: "+implode((mixed)parse_command_id_list(),", ")+"\n");
    write("Plurals: "+implode((mixed)parse_command_plural_id_list(),", ")+"\n");
    write("Adjectives: "+implode((mixed)parse_command_adjectiv_id_list(),", ")+"\n");
    write("Long: \n"+long());
    return 1;
}





/* Nasty hack for 3.2 */
string
long() { return description::long(); }

create(){
    parse_init();
    properties::create();
    description::create();
    flags::create();
    configure_set(STD_FLAGS, 0, 0, (: parse_refresh() :), 0, 0);
//    restore_object("/data/"+base_name(this_object()));
}
