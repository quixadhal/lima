/* Do not remove the headers from this file! see /USAGE for more info. */

inherit NVERB_OB;

void do_enter_obj(object ob, string s)
{
   environment(this_body())->do_go_obj(ob, 0);
}

void create()
{
    add_rules( ({ "OBJ" }), ({}));
}
