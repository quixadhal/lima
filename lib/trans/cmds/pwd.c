/* Do not remove the headers from this file! see /USAGE for more info. */

// Rust/Belboz

#include <mudlib.h>




int main()
{
  string cwf,pwd;

  cwf = this_body()->query_cwf();
  pwd = this_body()->query_pwd();
  if(!cwf) cwf = "NONE";
  if(!pwd) pwd = "NONE";

   printf("Dir:  %s\nFile: %s\n",pwd,cwf);
   return 1;
}

              
