#include <mudlib.h>
#include <security.h>

inherit M_ACCESS;

void create()
{
  set_privilege(1);
}
