/* Do not remove the headers from this file! see /USAGE for more info. */

/* 
 * Revised 2-14-99 by Tigran in his exit rewrite
 */

inherit NVERB_OB;

void do_climb_wrd_obj(string prep, object ob)
{
  ob->do_verb_rule("climb", "WRD OBJ", prep,ob);
}

void do_climb_obj(object ob)
{
  ob->do_verb_rule("climb","OBJ",ob);
}

void create()
{
   add_rules( ({ "OBJ", "WRD OBJ" }), ({ }) );
}
