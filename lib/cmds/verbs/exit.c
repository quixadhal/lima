/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NVERB_OB;

void do_exit()
{
   environment(this_body())->do_go_obj(environment(this_body()), 0);
}

void do_exit_obj(object ob)
{
   environment(this_body())->do_go_obj(ob, 0);
}

void create()
{
   add_rules( ({ "OBJ", "" }), ({ "leave", "disembark" }) );
}
