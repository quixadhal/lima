/* Do not remove the headers from this file! see /USAGE for more info. */

#include <move.h>
#include <hooks.h>

mixed call_hooks(string, int);

int
remove()
{
    if (environment())
        environment()->release_object(this_object());

//:HOOK remove
//Called when an object is removed.  The return value is ignored

    // Abstract class fix
    if (file_name() != "/std/object/non_object")
	call_hooks("remove", HOOK_IGNORE);
    destruct(this_object());
}

int receive_object( object target, string relation )
{
  return MOVE_NOT_ALLOWED;
}

int release_object( object target )
{
  return 0;
}
