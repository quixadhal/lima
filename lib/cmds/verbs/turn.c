/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

int do_turn_obj(object ob1)
{

    if(!ob1->turn()) 
      return 0;
}

array query_verb_info()
{
   return ({ ({ "OBJ" }) });
}
