/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit M_LOCKABLE;

void set_locked(string x)
{
    if (clonep(this_object()))
	base_name(this_object())->set_locked(x);

    ::set_locked(x);
}

string query_locked()
{
    if (clonep(this_object()))
	return base_name(this_object())->query_locked();

    return ::query_locked();
}
