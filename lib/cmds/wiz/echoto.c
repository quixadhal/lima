/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

private void main( mixed arg ) {
    printf("You echo to %s: %s\n", arg[0]->query_name(), arg[1]);
    tell_object(arg[0], arg[1] + "\n");
}
