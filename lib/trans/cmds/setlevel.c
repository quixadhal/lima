/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit DAEMON;


int main(mixed * arg)
{
  if(!GROUP_D->adminp(previous_object(1))) return 0;

  arg[0]->set_level(arg[1]);
  arg[0]->save_me();
  return 1;
}
