/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;



private void main( string message )
{
  string array exits = environment(this_body())->query_exit_directions();
  if(!sizeof(exits))
  {
    out("There aren't any exits that you can see.\n");
    return;
  }
  out(iwrap(implode(environment(this_body())->query_exit_directions(),
	", ")+".\n"));
}
