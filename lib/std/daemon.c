/* Do not remove the headers from this file! see /USAGE for more info. */

#include <security.h>

inherit M_ACCESS;

void clean_up() {
    if (!environment() && !first_inventory(this_object()))
	destruct(this_object());
}

create() {
  set_privilege(1);
}
