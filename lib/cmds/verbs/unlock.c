/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit VERB_OB;
inherit M_PARSING;

void do_unlock_obj_with_obj(object ob1, object ob2) {
    ob1->unlock_with(ob2);
} 

mixed do_unlock_obj(object ob)
{
    ob->unlock();
}

mixed *query_verb_info()
{
  return ({ ({ "OBJ", "OBJ with OBJ" }) });
}
