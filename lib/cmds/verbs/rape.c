/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <setbit.h>

inherit VERB_OB;

mixed can_rape_obj(object ob)
{
    return 1;
}

void do_rape_obj(object ob)
{
    if (ob->is_living())
    {
	write( "Now now. Play nice.\n" );
    }
    else
    {
	write( "What an <ahem> strange idea.\n" );
    }
}

mixed * query_verb_info()
{
    return ({ ({ "OBJ" }) });
}
