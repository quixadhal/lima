/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

#include <setbit.h>
inherit VERB_OB;

mixed can_close_obj(object ob)
{
   return 1;
}

mixed do_close_obj(object ob) {
    ob->close();
}

mixed * query_verb_info() {
  return ({ ({ "OBJ" }) });
}
