/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** inside.c
**
** The inside of an object that resides within a room.  This is actually
** done by having two objects -- the inside and the outside.
**
** 951123, Deathblade: created
*/

inherit ROOM;

private object outside;

object get_outside()
{
    return outside;
}
void set_outside(object new_outside)
{
    outside = new_outside;
}

string exterior_view()
{
    if ( outside )
	return iwrap("\nOutside you see:\n" + environment(outside)->long());

    return "";
}

//### damn... rooms don't normally get messages...
private nomask void catch_tell(string message)
{
    if ( outside )
	tell_room(environment(outside), "^ "+message, ({ outside }));
}

varargs void create(object the_outside)
{
    ::create();

    outside = the_outside;	// note: may be 0

    // default exit... may be overriden by subclasses...
    set_exits(([ "out" : (: environment(get_outside()) :) ]));

    add_hook("extra_long", (: exterior_view :));
}
