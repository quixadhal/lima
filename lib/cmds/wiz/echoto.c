/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

void create()
{
  ::create();
  no_redirection();
}

private void main( string orig_input, mixed arg, mapping flags){

     string msg = orig_input[strsrch(orig_input," ")+1..];
    outf("You echo to %s: %s\n", arg[0]->query_name(), msg);
    tell(arg[0], msg + "\n");
}
