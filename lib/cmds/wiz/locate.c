/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
inherit CMD;

private void main(string* arg)
{

    IMUD_D->do_locate(arg[0]);

    write("Locate query sent.\n");
}
