/* Do not remove the headers from this file! see /USAGE for more info. */

#include <verbs.h>

inherit NVERB_OB;

void do_eat_obj( object ob )
{
    ob->do_eat();
}

void create()
{
    add_flag(TRY_TO_ACQUIRE);
    
    add_rules( ({ "OBJ" }), ({ "bite" }) );
}
