/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** pick.c
**
*/

#include <move.h>
#include <setbit.h>

inherit NVERB_OB;

void do_pick_obj(object ob)
{
  ob->pick();
}

void do_pick_obj_with_obj(object ob1, object ob2)
{
  ob1->open_with(ob2);
}

void create()
{
  add_rules(({ "OBJ", "OBJ with OBJ" }));
}
