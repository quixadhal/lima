/* Do not remove the headers from this file! see /USAGE for more info. */


inherit VERB_OB;

void do_wield_obj(object ob)
{
    if (!try_to_acquire(ob))
	return;
    
    this_body()->do_wield(ob);
}
 
array query_verb_info ()
{
    return ({ ({ "OBJ" }) });
}
