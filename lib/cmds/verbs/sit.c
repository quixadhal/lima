/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** sit.c
*/

inherit NVERB_OB;

void do_sit_wrd_obj(string prep, object ob)
{
   environment(this_body())->do_go_obj(ob, prep);
}

void create()
{
   add_rules( ({ "WRD OBJ" }), ({ }) );
}
