/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NVERB_OB;

void do_climb_wrd_obj(string prep, object ob)
{
   environment(this_body())->do_go_obj(ob, prep);
}

void do_climb_obj(object ob)
{
   environment(this_body())->do_go_obj(ob, 0);
}

void create()
{
   add_rules( ({ "OBJ", "WRD OBJ" }), ({ }) );
}
