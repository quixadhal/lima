/* Do not remove the headers from this file! see /USAGE for more info. */

#include <move.h>
#include <hooks.h>
#include <setbit.h>

mixed call_hooks(string, int);
void assign_flag(string, int);
int test_flag(string);
void set_light(int);

//:FUNCTION
//This function is guaranteed to be called when an object is destructed.
//It tidies up some things like updating its environment's capacity and
//light level.  It also calls the "remove" hook.
int remove()
{
    if (environment())
        environment()->release_object(this_object(), 1);

    // Abstract class fix
    if ( file_name() != __DIR__ "non_object" )
    {
//:HOOK remove
//Called when an object is removed.  The return value is ignored
	call_hooks("remove", HOOK_IGNORE);

	/* turn off any light we might be providing. bad to leave it on :-) */
	set_light(0);
    }

    destruct();
}

//:FUNCTION receive_object
//receive_object(obj, relation) should return 1 if obj can be put ("in", "on",
//etc) the object, where 'relation' specifies the relation.  By default,
//objects cannot contain things so MOVE_NOT_ALLOWED is returned.  See also
//the container inheritable.
mixed receive_object( object target, string relation )
{
    return MOVE_NOT_ALLOWED;
}

varargs mixed release_object( object target, int force )
{
    return 1;
}

//:FUNCTION set_attached
//set_attached(1) or set_attached() sets the ATTACHED flag for this 
//object, and set_attached(0) removes it.
varargs void set_attached(int a)
{
    if( !a )
        a = 1;
    assign_flag(ATTACHED, a);
}

//:FUNCTION is_attached
//returns 1 if the ATTACHED flag is set for this object.
int is_attached()
{
    return test_flag(ATTACHED);
}
