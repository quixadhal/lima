/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

 
inherit VERB_OB;
 
void do_wind_obj(object ob) {
    ob->do_wind();
}

void do_wind_obj_with_obj(object ob1, object ob2) {
    ob2->do_wind(ob1);
}
 
void create() {
    add_rules( ({ "OBJ", "OBJ with OBJ" }) );
}
