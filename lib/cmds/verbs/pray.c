/* Do not remove the headers from this file! see /USAGE for more info. */

#include <verbs.h>
inherit VERB_OB;

void do_pray_to_obj(object ob)
{
  ob->do_pray();
}

void do_pray_obj(object ob)
{
  ob->do_pray();
}

void do_pray()
{
  environment(this_body())->do_pray();
}

void create()
{
  add_rules( ({ "OBJ", "", "to OBJ" }) );
  clear_flag(NEED_TO_BE_ALIVE);
}
