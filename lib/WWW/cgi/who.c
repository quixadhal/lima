/* Do not remove the headers from this file! see /USAGE for more info. */

#include <daemons.h>

string main(string user) 
{
  string	ret;

  user = lower_case (trim_spaces (user));
  ret = FINGER_D->get_finger (user, 1);

  if(user == "")
    {
      return ret;
    }

  user = capitalize (user);

  ret = sprintf ("<title>Finger info for %s@%s</title>\n"
        "<h2>Finger info for %s@%s</h2>\n%s\n", user, MUD_NAME,
	user, MUD_NAME, ret);
  
  return ret;
}
