/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

mixed can_enter_obj(object ob)
{
   return 1;
}
 
mixed do_enter_obj(object ob)
{
   if(!ob->board(ob)) return 0;
   return 1;
}

mixed *query_verb_info()
{
   return ({ ({ "OBJ" }) });
}
