/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>

inherit CMD;

void create()
{
  ::create();
  no_redirection();
}

private void main(string str) {
    if(!is_directory(wiz_dir(this_body())))
      {
	out("Sorry, only full wizards may use the I command.\n");
	return;
      }
    if (!str || str == "") {
         out("I <whatever you did>\n");
         return;
    }
    DID_D->someone_did(str);
    out("Reported.  (Thank you!)\n");
    return;
}

