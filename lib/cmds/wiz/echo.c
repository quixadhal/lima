/* Do not remove the headers from this file! see /USAGE for more info. */

// See echoall header. P-)

#include <mudlib.h>

inherit CMD;


private void main( string str) {

  if ( !str )
  {
    out("Echo what?\n");
    return;
  }

  if(!end_of_pipeline())
  {
     out(str);
     return;
  }

    tell_environment(this_body(), str + "\n", 0, ({ this_body() }));
    write("You echo: "+str+"\n");

    return;
}
