/* Do not remove the headers from this file! see /USAGE for more info. */

// attributes_d.c - Seperated from the old M_OBJ_ATTRIBUTES.
// June 30, 1998: Iizuka.

#include <flags.h>

private nosave mapping attribute_info = ([
        F_OPEN    : "opened_attributes",
        F_LIGHTED : "lighted_attributes",
        F_WIELDED : "wielded_attributes",
        F_WORN    : "worn_attributes",
     ]);

mapping get_global_attributes()
{
   return attribute_info;
}
