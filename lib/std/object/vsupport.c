/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** ### HACK!  temporary until we get verb support fully integrated
**
** Each of these groups of functions should move to whatever module
** supports the particular verb.  For example, the "give" functions
** would probably move to M_GETTABLE, and the "read" functions to
** M_READABLE.
*
* -Beek: A few things should remain here.  direct_look_at_obj(), for
* example.  Most should leave tho.
*/

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
     
mixed direct_put_obj_in_obj(object ob1, object ob2) {
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

 
mixed direct_drop_wrd_wrd_wrd(string type)
{
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
