/* Do not remove the headers from this file! see /USAGE for more info. */

/* ### probably shouldn't be here (simul conversion) */
#include <mudlib.h>
#include <move.h>
#include <hooks.h>

inherit "/std/object/light";

private object	last_location;

void call_hooks(string, int);

varargs int move(mixed dest, string where)
{
    object env;
    object tmp;
    int ret;
    mixed err;

//:HOOK prevent_drop
//A yes/no/error type hook that can be used to prevent the object from being
//moved out of it's environment.  The error value is discarded.    
    err = call_hooks("prevent_drop", HOOK_YES_NO_ERROR);
    if (err == 0 || stringp(err)) return MOVE_NOT_ALLOWED;

    if( stringp(dest) )
	dest = load_object( dest );

    if (!objectp(dest)) return MOVE_NO_DEST;

    env=environment();
    if (env && ret = env->release_object( this_object() ) )
        return (ret == -1 ? ret : MOVE_NOT_RELEASED);
    if (ret = dest->receive_object( this_object(), where ) )
    {
	if( env )
	    env->update_capacity();
        return ret;
    }

    last_location=env;

    if (query_light() && (tmp = our_room())) {
        tmp->adjust_light(-(int)query_light());
    }

    move_object(dest);
//:HOOK move
//Called when an object moves.  The return value is ignored.
    call_hooks("move", HOOK_IGNORE);
    
    if (query_light() && (tmp = our_room())) {
        tmp->adjust_light(query_light());
    }

    /* doing things (like desting the object) above in
     * in receive_object would be bad.  Give the destination
     * a chance to do what it wants
     */

//:HOOK object_left
//Called when an object leaves an that object.  The return value is ignored.
//The object moving is passed as the first arg.

//:HOOK object_arrived
//Called when an object arrives in an object.  The return value is ignored.
//The object moving is passed as the first arg.

    if (env) env->call_hooks("object_left", HOOK_IGNORE, 0, this_object());
    dest->call_hooks("object_arrived", HOOK_IGNORE, 0, this_object());
    return MOVE_OK;
}

object query_last_location()
{
    return last_location;
}
