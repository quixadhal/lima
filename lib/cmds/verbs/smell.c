/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** smell.c
**
*/

#include <mudlib.h>

inherit VERB_OB;

mixed can_smell_obj(object ob)
{
    return 1;
}

mixed direct_smell_obj(object ob) {
    return 1;
}

void do_smell_obj(object ob)
{
    if ( !ob->smell() )
    {
	printf("It smells pretty much like %s.  What did you expect?\n", ob->a_short());
    }
}


mixed * query_verb_info()
{
    /* ### conflicts with emote... giving preference to emote for now */
//    return ({ ({ "OBJ" }), ({ "sniff" }) });

    return ({ ({ "OBJ" }) });
}
