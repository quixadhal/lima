/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

 
inherit VERB_OB;
 
void do_wind_obj(object ob) {
    ob->wind();
}

void do_wind_obj_with_obj(object ob1, object ob2) {
    ob2->use("wind", ob1);
}
 
array query_verb_info()
{
  return ({ ({ "OBJ", "OBJ with OBJ" }) });
}
