/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

int main( mixed *arg )
{
    object	target;
    string func;
    string ret;

    target = arg[1];
    func = arg[0];
   ret = function_exists(func, target);
   if (ret) {
      printf("Function defined in %s.\n", ret);
   } else {
      printf("Function not defined in that object.\n");
   }
   return 1;
}
