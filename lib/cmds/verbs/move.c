/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit M_PARSING;
inherit VERB_OB;

void do_move_obj(object ob)
{
    ob->disturb();
}

array query_verb_info()
{
    return ({ ({ "OBJ" }) });
}
