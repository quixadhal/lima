/* Do not remove the headers from this file! see /USAGE for more info. */

#include "mudlib.h"
inherit CMD;

private void main( mixed *arg)
{
    this_body()->set_light(arg[0]);
    printf("%i light added.\n", arg[0]);
}
