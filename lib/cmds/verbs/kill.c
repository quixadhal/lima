/* Do not remove the headers from this file! see /USAGE for more info. */


#include <mudlib.h>

inherit VERB_OB;

mixed can_kill_liv(object ob)
{
    return check_vision();
}

void do_kill_liv(object ob)
{
    this_body()->start_fight(ob);
}

mixed * query_verb_info()
{
   return ({ ({ "LIV" }) });
}

