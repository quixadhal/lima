/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit VERB_OB;
inherit M_PARSING;

void do_lock_obj_with_obj(object ob1, object ob2) {
    ob1->lock_with(ob2);
}

void do_lock_obj(object ob)
{
    ob->lock(ob);
}

array query_verb_info()
{
    return ({ ({ "OBJ", "OBJ with OBJ" }) });
}
