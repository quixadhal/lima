/* Do not remove the headers from this file! see /USAGE for more info. */

/* 
 * Written by Tigran, September 7, 2000
 */

inherit VERB_OB;

void do_ascend_obj(object ob)
{
  ob->do_verb_rule("ascend","OBJ",ob);
}

void create()
{
   add_rules( ({ "OBJ", }), ({ }) );
}
