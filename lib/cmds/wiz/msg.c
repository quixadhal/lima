/* Do not remove the headers from this file! see /USAGE for more info. */
#include <mudlib.h>

inherit CMD;


private void main( mixed arg)
{
   string value;
value = implode(arg[1], " ");
    if (value == "clear") this_body()->set_msgs(arg[0], ({}));
else this_body()->add_msg(arg[0], value +"\n");
write("Ok.\n");
return 1;
}
