/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <hooks.h>

varargs mixed call_hooks(string, int, mixed);

mixed direct_look_at_obj(object ob) {
    return 1;
}

mixed direct_look_obj(object ob) {
    return 1;
}

mixed direct_smell_obj(object ob)
{
    return 1;
}

mixed direct_give_obj_to_liv(string rule)
{
    return 1;
}

mixed direct_eat_obj(object ob) {
    return "I don't think " + ob->the_short() + " would agree with you.\n";
}

mixed direct_look_str_obj(string prep, object ob) {
    return "There is nothing " + prep + " " + ob->the_short() + ".\n";
}
     
mixed direct_put_obj_wrd_obj(object ob1, object ob2) {
    return 1;
}

mixed direct_get_obj(object ob)
{
    if ( environment() == this_body() )
	return "You already have it!\n";
    if ( environment()->is_living() )
	return "Too bad you're not a skilled pickpocket.\n";
    if ( this_object() == this_body() )
	return "You make an advance on yourself.\n";

//:HOOK prevent_get
//A yes/no/error hook called by direct_get_obj() if the standard conditions
//succeed

    return call_hooks("prevent_get", HOOK_YES_NO_ERROR);
}

mixed direct_get_obj_from_obj(object ob1, object ob2) {
    return direct_get_obj(ob1);
}

mixed direct_get_obj_out_of_obj(object ob1, object ob2) {
    return direct_get_obj(ob1);
}

mixed direct_get_obj_with_obj(object ob1, object ob2) {
    return direct_get_obj(ob1);
}

mixed direct_sell_obj_to_liv(object ob, object liv) {
    if (!ob) return 1;
    if (owner(ob) != this_body())
	return "You don't have it!\n";
    if(!ob->query_value()) return "That item has no value!\n";
    return 1;
}
 
mixed direct_buy_obj_from_liv(object ob, object liv) {
    if (owner(ob) != liv)
	return liv->query_possessive() + "doesn't have that!\n";
    return 1;
}
 
mixed direct_drop_obj(object ob)
{
    if ( environment() != this_body() )
	return "You don't have it!\n";


//:HOOK prevent_drop
//A yes/no/error hook called by direct_drop_obj() if the standard conditions
//succeed

    return call_hooks("prevent_drop", HOOK_YES_NO_ERROR);
}

mixed direct_flip_obj(object ob) {
//:HOOK direct_flip
//A yes/no/error hook called by direct_flip_obj(); if no hooks exists, the
//useless message will be used.

    return call_hooks("direct_flip", HOOK_YES_NO_ERROR,
		     M_PARSING->useless("Fiddling with " + ob->the_short()));
}


mixed direct_throw_obj(object ob1, object ob2)
{
  return 1;
}

mixed direct_throw_obj_at_obj(object ob1, object ob2)
{
  return 1;
}

 mixed indirect_throw_obj_at_obj(object ob1, object ob2)
 {
   return 1;
 }


