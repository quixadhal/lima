/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_tie_obj_to_obj(object ob1, object ob2)
{
  ob1->do_tie(ob2);
}

void do_tie_obj(object ob)
{
  ob->do_tie();
}

void create()
{
  add_rules( ({ "OBJ", "OBJ to OBJ" }) );
}
