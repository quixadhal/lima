/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit CMD;


private void main(string arg)
{
    object* u;
    int i;
    if(!arg)
      {
	write("Wizcall what?\n");
	return;
      }
    u = filter_array(users(), (: wizardp :));
    i = sizeof(u);
    if(!i){
	write("You get the feeling no one hears you.\n");
	return;
    }
    while(i--)
	tell_object(u[i], iwrap(sprintf("%s wizcalls: %s\n",
					this_body()->query_name(), arg)));
    printf("You wizcall: %s\n",arg);
    return;
}
