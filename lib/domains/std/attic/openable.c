/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit M_OPENABLE;

void set_closed(int x) {
  if (clonep(this_object())) base_name(this_object())->set_closed(x);
  else ::set_closed(x);
}

int query_closed() {
  if (clonep(this_object())) return base_name(this_object())->query_closed();
  else return ::query_closed();
}
