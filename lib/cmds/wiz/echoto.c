/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

private void main( mixed arg ) {
    string msg = implode(arg[1], " ");
    printf("You echo to %s: %s\n", arg[0]->query_name(), msg);
    tell_object(arg[0], msg + "\n");
}
