/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** stand.c

   Added standing in and standing on objects
   Vette  April 11, 1997
*/

inherit NVERB_OB;

mixed can_stand()
{
   return 1;
}

void do_stand()
{
   environment(this_body())->do_go_obj(environment(this_body()), 0);
}

void do_stand_wrd_obj(string prep, object ob)
{
   environment(this_body())->do_go_obj(ob, prep);
}

void create()
{
   add_rules( ({ "" }) );
   add_rules( ({ "WRD" }), ({ "get" }) );
   add_rules( ({ "WRD OBJ" }), ({ }) );
}
