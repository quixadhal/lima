/* Do not remove the headers from this file! see /USAGE for more info. */

#include <move.h>

int
remove()
{
    if (environment())
	environment()->release_object(this_object());
    destruct(this_object());
    return 0;
}

int receive_object( object target, string relation )
{
  return MOVE_NOT_ALLOWED;
}

int release_object( object target )
{
  return 0;
}
