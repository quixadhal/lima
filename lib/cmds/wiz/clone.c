/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>

inherit DAEMON;



int main( mixed *arg ) {
    object o;

    o = clone_object(arg[0]);
    if( !o )
    {
	write("Failed to load file.\n");
	return 1;
    }
    this_body()->do_player_message("clone", o);
    if (o->get() <= 0 || o->move(this_body())) {
	if (o->move(environment(this_body())))
	    printf("Ok. (It couldn't be moved.)\n");
	else
	    printf("Ok. (It is on the ground.)\n");
    } else
	printf("Ok. (It is in your inventory.)\n");
    return 1;
}

int help()
{
    write(wrap(
	"This command creates a copy of the object whose file name is "
	"passed to this command.  If the object is cloned to the room "
	"you're in, it can't be carried, otherwise it'll end up in "
	"your inventory.\n"
      ));
    return 1;
}
