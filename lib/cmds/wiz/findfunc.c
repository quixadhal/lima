/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;

private void main( mixed *arg )
{
    object	target;
    string func;
    string ret;

    target = arg[1];
    func = arg[0];
   ret = function_exists(func, target);
   if (ret) {
      outf("Function defined in %s.\n", ret);
   } else {
      out("Function not defined in that object.\n");
   }
}
