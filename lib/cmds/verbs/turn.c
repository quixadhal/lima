/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_turn_obj(object ob)
{
    ob->turn();
}

array query_verb_info()
{
   return ({ ({ "OBJ" }) });
}
