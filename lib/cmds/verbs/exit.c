/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NVERB_OB;

void do_exit()
{
   environment(this_body())->do_verb_rule("exit");
}

void do_exit_obj(object ob)
{
   environment(this_body())->do_verb_rule("exit","OBJ",ob);
}

void create()
{
   add_rules( ({ "OBJ", "" }), ({ "leave", "disembark" }) );
}
