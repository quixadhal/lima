/* Do not remove the headers from this file! see /USAGE for more info. */

// Davmar/Nevyn

#include <mudlib.h>

inherit CMD;


private void main( mixed *arg )
{
    object ob;

    ob = arg[0];

    if (ob->short()) {
	this_body()->do_player_message("destruct", ob);
    } else {
	write("Destructing: " + file_name(ob) + "\n");
    }
    if(member_array(ob,bodies()) != -1)
      ob = ob->query_link();
    destruct(ob);
    return;
}
