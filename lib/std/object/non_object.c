/* Do not remove the headers from this file! see /USAGE for more info. */

#include <move.h>
#include <hooks.h>

mixed call_hooks(string, int);

int
remove()
{
    if (environment())
        environment()->release_object(this_object(), 1);

//:HOOK remove
//Called when an object is removed.  The return value is ignored

    // Abstract class fix
    if (file_name() != "/std/object/non_object")
	call_hooks("remove", HOOK_IGNORE);
    destruct(this_object());
}

mixed receive_object( object target, string relation )
{
  return MOVE_NOT_ALLOWED;
}

varargs mixed release_object( object target, int force )
{
  return 1;
}
