/* Do not remove the headers from this file! see /USAGE for more info. */

/* 
 * Written 2-16-99 by Tigran
 */

inherit NVERB_OB;

void do_crawl_wrd_obj(string prep, object ob)
{
  ob->do_verb_rule("crawl", "WRD OBJ", prep,ob);
}

void create()
{
   add_rules( ({ "WRD OBJ" }), ({ }) );
}
