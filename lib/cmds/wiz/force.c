/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
// I don't think this works...

inherit DAEMON;



#define SYNTAX "Usage:  force <living> <command>\n"
int main(mixed * arg)
{
  string what = implode(arg[1], " ");

   tell_object(arg[0], this_body()->query_name()+" forced you to: "+what+"\n");
   arg[0]->query_link()->force_me(what);
   return 1;
}
