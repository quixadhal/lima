/* Do not remove the headers from this file! see /USAGE for more info. */

//### probably shouldn't be here (simul conversion)
#include <mudlib.h>
#include <move.h>
#include <hooks.h>


private object last_location;

void call_hooks(string, int);
int query_light();

int contains(object o)
{
    return o->is_in(this_object());
}
int is_in(object o)
{
    object env = this_object();
    while(env = environment(env))
    {
	if(env == o)
	    return 1;
    }
}


varargs mixed move(mixed dest, string where)
{
    object env;
    object tmp;
    mixed ret;
    mixed err;
    int light;

//:HOOK prevent_drop
//A yes/no/error type hook that can be used to prevent the object from being
//moved out of it's environment.  The error value is discarded.    
    err = call_hooks("prevent_drop", HOOK_YES_NO_ERROR);
    if (err == 0) err = MOVE_PREVENTED;
    if (stringp(err)) return err;

    if( stringp(dest) )
	dest = load_object( dest );

    if (!objectp(dest)) return MOVE_NO_DEST;

    if (dest->is_in(this_object()))
       return "You can't move an object inside itself.\n";
    env=environment();
    if (env) {
        ret = env->release_object( this_object() );
        if (ret == 0) ret = MOVE_NOT_RELEASED;
        if (stringp(ret)) return ret;
    }
    ret = dest->receive_object( this_object(), where );
    if (ret == 0) ret = MOVE_NOT_RECIEVED;
    if (stringp(ret)) {
	if( env )
	    env->update_capacity();
        return ret;
    }

    last_location=env;

    if ( (light = query_light()) && env )
	env->containee_light_changed(-light);

    move_object(dest);
//:HOOK move
//Called when an object moves.  The return value is ignored.
    call_hooks("move", HOOK_IGNORE);

    /* requery light just in case a hook changed it (e.g. a sword stopped
       glowing when released) */
    if ( (light = query_light()) && dest )
	dest->containee_light_changed(light);

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
