/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_drink_obj(object ob)
{
    if (!try_to_acquire(ob))
	return;
    ob->drink_it();
}

void do_drink_from_obj(object ob)
{
   if(!try_to_acquire(ob))
    return;
   ob->drink_from_it();
}

array query_verb_info()
{
      return ({ ({ "OBJ", "from OBJ" }) });
}
