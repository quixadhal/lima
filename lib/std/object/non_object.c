/* Do not remove the headers from this file! see /USAGE for more info. */

#include <move.h>
#include <hooks.h>
#include <setbit.h>

mixed call_hooks(string, int);
void assign_flag(string, int);
int test_flag(string);
void set_light(int);

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

mixed receive_object( object target, string relation )
{
    return MOVE_NOT_ALLOWED;
}

varargs mixed release_object( object target, int force )
{
    return 1;
}

void set_attached(int a)
{
    assign_flag(ATTACHED, a);
}

int is_attached()
{
    return test_flag(ATTACHED);
}
