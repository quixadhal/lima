/* Do not remove the headers from this file! see /USAGE for more info. */

// See echoall header. P-)

#include <mudlib.h>




int main( string str) {

  if ( !str )
  {
    write("Echo what?\n");
    return 1;
  }

    tell_room(environment(this_body()), str + "\n", ({ this_body() }));
    write("You echo: "+str+"\n");

    return 1;
}
