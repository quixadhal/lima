/* Do not remove the headers from this file! see /USAGE for more info. */

//   The following is an attempt at a value module.
//
#include <mudlib.h>

int object_value = 0;

void set_value(int value)
{
    object_value = value;
}

int query_value()
{
    return object_value;
}
