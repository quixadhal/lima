/* Do not remove the headers from this file! see /USAGE for more info. */

// Used to create paramterised objects where the only change made is
// via the base id being passed in the create.
// Used in /domains/std/recorder/button.

inherit OBJ;

object virtual_create(string args)
{
    return new(base_name(), args);
}
