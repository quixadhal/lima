/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_enter_obj(object ob)
{
  ob->do_verb_rule("enter","OBJ",ob);
}

void create()
{
    add_rules( ({ "OBJ" }), ({}));
}
