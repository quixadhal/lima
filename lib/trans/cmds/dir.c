/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <commands.h>
inherit DAEMON;

void main(mixed argv, mapping f)
{
  CMD_OB_LS->main(argv,f);
}
