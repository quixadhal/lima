/* Do not remove the headers from this file! see /USAGE for more info. */

//**********************************/
//*  move                          */
//**********************************/

#include <mudlib.h>

inherit VERB_OB;

mixed can_wield_obj()
{
    return 1;
}

void do_wield_obj(object ob)
{
    this_body()->do_wield(ob);
}
 
mixed * query_verb_info ()
{
    return ({ ({ "OBJ" }) });
}
