/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_turn_obj(object ob)
{
  ob->do_turn();
}

void do_turn_str_wrd_obj(string str, string wrd, object ob)
{
  ob->do_turn(str, wrd);
}

void do_turn_str_wrd_obj_wrd(string str, string wrd, object ob, string wrd2)
{
  ob->do_turn(str, wrd, wrd2);
}

void create()
{
 add_rules( ({ "OBJ", "STR WRD OBJ", "STR WRD OBJ WRD", }) );
}
