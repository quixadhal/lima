/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** outside.c
**
** Give an object the ability to be entered.  This will transport the
** player's body to another room that is "inside" of this object.
** Additional facilities are provided by this module to help with
** maintaining the relationship between the room inside and this object
** and this object's exterior.
*/

void add_hook(string tag, function hook_func);

private mixed inside_dest;

private void remove_dest()
{
    if ( objectp(inside_dest) )
	inside_dest->remove();
}

varargs void set_inside_destination(mixed dest, int auto_clone)
{
    if ( auto_clone && stringp(dest) )
    {
	dest = new(dest, this_object());
	add_hook("remove", (: remove_dest :));
    }

    inside_dest = dest;
}
mixed get_inside_destination()
{
    return inside_dest;
}

mixed direct_enter_obj()
{
//###what else needs to happen in here?
    if ( inside_dest )
	return 1;
    return 0;
}

int do_enter()
{
    this_body()->move_to(inside_dest, "enter");
}

private nomask void catch_tell(string message)
{
//### actually, we need to evaluate this thing in some way
    if ( objectp(inside_dest) )
	tell_room(inside_dest, "^ "+message, ({ }));
}
