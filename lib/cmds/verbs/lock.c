/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit VERB_OB;
inherit M_PARSING;

mixed can_lock_obj(object ob)
{
    return 1;
}

mixed can_lock_obj_with_obj( object ob1, object ob2)
{
return 1;
}


void do_lock_obj_with_obj(object ob1, object ob2) {
ob1->lock_with(ob2);
} 

void do_lock_obj(object ob)
{
  if (!ob->lock(ob))
	write ( useless ("You cant lock "+ ob->the_short()));
}
mixed *query_verb_info()
{
    return ({ ({ "OBJ", "OBJ with OBJ" }) });
}
