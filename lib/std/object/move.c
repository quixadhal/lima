/* Do not remove the headers from this file! see /USAGE for more info. */

/* ### probably shouldn't be here (simul conversion) */
#include <mudlib.h>
#include <move.h>

inherit "/std/object/light";

private object	last_location;

varargs int move(mixed dest, string where)
{
    object env;
    object tmp;
    int ret;

    if (this_object()->prevent_drop()) return MOVE_NOT_ALLOWED;

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

    if (query_light() && (env = our_room())) {
        env->adjust_light(-(int)query_light());
    }

    move_object(dest);

    if (query_light() && (env = our_room())) {
        env->adjust_light(query_light());
    }

    /* doing things (like desting the object) above in
     * in receive_object would be bad.  Give the destination
     * a chance to do what it wants
     */
    dest->extra_receive(this_object(), where);
    return MOVE_OK;
}

object query_last_location()
{
    return last_location;
}
