/* Do not remove the headers from this file! see /USAGE for more info. */

// Onyx @ Red Dragon Nov. 1995

#include <mudlib.h>

inherit CMD;

#define MAKER "/trans/obj/buildmenu"

private void main()
{
    new(MAKER)->start_menu();
}
