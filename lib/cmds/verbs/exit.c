/* Do not remove the headers from this file! see /USAGE for more info. */


#include <mudlib.h>

inherit VERB_OB;


mixed can_exit_obj(object ob)
{
   return 1;
}

mixed do_exit_obj(object ob)
{
   return ob->exit();
}

mixed * query_verb_info()
{
   return ({ ({ "OBJ" }) });
}
