/* Do not remove the headers from this file! see /USAGE for more info. */

//### probably shouldn't be here (simul conversion)
#include <move.h>
#include <hooks.h>


private object last_location;

void call_hooks(string, int);
int query_light();

//### move these next to /std/object/environment, and merge with some of
//### the objcalls simuls
//:FUNCTION contains
//contains(o) returns 1 if o is in this object, or in an object contained
//by this object (recursively).
int contains(object o)
{
    return o->is_in(this_object());
}

//:FUNCTION is_in
//is_in(o) returns 1 if this object is in o, or in an object which is in
//o (recursively).
int is_in(object o)
{
    object env = this_object();
    while(env = environment(env))
    {
	if(env == o)
	    return 1;
    }
}

//:FUNCTION move
//move(dest, relation) moves this object to be 'relation' ("in", "on", etc)
//to the object dest, if possible.  If it fails, it returns a string
//error message or zero; if it succeeds it returns 1.
varargs mixed move(mixed dest, string where)
{
    object env;
    object tmp;
    mixed ret;
    mixed err;
    int light;

    if (environment()) {
	//:HOOK prevent_drop
	//A yes/no/error type hook that can be used to prevent the object from being
	//moved out of it's environment.  The error value is discarded.    
	err = call_hooks("prevent_drop", HOOK_YES_NO_ERROR);
	if (err == 0) err = MOVE_PREVENTED;
	if (stringp(err)) return err;
    }

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
    if (ret == 0) ret = MOVE_NOT_RECEIVED;
    if (stringp(ret)) {
	if( env )
	{
	    env->reinsert_object(this_object(), where);
	    env->update_capacity();
	}
	return ret;
    }

    last_location=env;

    if ( (light = query_light()) && env )
	env->containee_light_changed(-light);

    move_object(dest);
#ifdef USE_STATUS_LINE
    if( this_object()->has_status_line())
	call_out( (: this_object()->update_status_line() :), 0);

    ret = all_inventory( this_object());
    foreach( tmp in ret )
    {
	if( tmp->has_status_line()) tmp->update_status_line();
    }
#endif

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

//### IMO this is only used for hacks, and should be removed (-Beek)
//:FUNCTION query_last_location
//Returns the last object an object was inside before it moved to where
//it is now.
object query_last_location()
{
    return last_location;
}


object give_new_obj( object target, string obj, int recurse )
{
    object ob = new( absolute_path(obj));

    if( !ob )
	error( "give_new_obj() failed: No such object " + obj + ".\n" );
    if( !target )
	error( "give_new_obj() failed: Invalid target.");

    while(1)
    {
	if( ob->move( target ))
	    return ob;
	if( recurse && target = environment( target )) continue;
	error( "give_new_obj() failed: No room for object anywhere (recursed).");
    }
}
