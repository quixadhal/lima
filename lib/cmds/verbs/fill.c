/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <setbit.h>

inherit M_PARSING;
inherit VERB_OB;

mixed can_fill_obj(object ob)
{
    return 1;
}

mixed can_fill_obj_with_obj(object ob1, object ob2) {
    return 1;
}

int do_fill_it(object ob, object with)
{
    if (!with) {
	with = present("water", this_body())
	|| present("water", environment(this_body()));
	if (!with) {
	    write("With what?\n");
	    return 1;
	}
	write("(with water)\n");
    }
    ob->fill_with(with);
}

void do_fill_obj(object ob)
{
    do_fill_it(ob, 0);
}

void do_fill_obj_with_obj(object ob1, object ob2) {
    do_fill_it(ob1, ob2);
}

mixed * query_verb_info(string rule)
{
   return ({ ({ "OBJ", "OBJ with OBJ" }) });
}
