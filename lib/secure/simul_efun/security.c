/* Do not remove the headers from this file! see /USAGE for more info. */

#include <security.h>

nomask int check_previous_privilege(mixed priv)
{
  return SECURE_D->check_privilege(priv,2);
}

nomask int check_privilege(mixed priv)
{
  return SECURE_D->check_privilege(priv,1);
}

nomask mixed get_protection(string file,string mode)
{
  int rw;
  switch (mode[0])
  {
    case 'w':
      rw = 1;
      break;
    case 'r':
      rw = 0;
      break;
    default:
      error("Bad argument 2 to get_protection()\n");
      break;
  }
  switch (mode[1])
  {
    case 'f':
      file += "foo";
      break;
    case 'd':
      if (file[<1]=='/')
	file += "foo";
      else
	file += "/foo";
      break;
    default:
      error("Bad argument 2 to get_protection()\n");
      break;
  }
  return SECURE_D->query_protection(file,rw);
}

mixed get_privilege(object ob)
{
  return function_exists("query_privilege",ob)==M_ACCESS && ob->query_privilege();
}
