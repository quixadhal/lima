/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <daemons.h>

int main(string str) {
    if(!is_directory(wiz_dir(this_body())))
      {
	write("Sorry, only full wizards may use the I command.\n");
	return 1;
      }
    if (!str || str == "") {
         write("I <whatever you did>\n");
         return 0;
    }
    DID_D->someone_did(str);
    write("Reported.  (Thank you!)\n");
    return 1;
}

