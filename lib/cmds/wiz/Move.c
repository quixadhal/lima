/* Do not remove the headers from this file! see /USAGE for more info. */

//Megaboz@ZorkMUD

#include <mudlib.h>
inherit CMD;

private void main( mixed *arg )
{
//### The room where the object was probably needs a message too
    arg[0]->move(arg[1]);
    tell(arg[0], "You are being moved to " + arg[1]->short() + "\n");
    tell_from_inside(arg[1], arg[0]->a_short() + " has been magically transported here.\n", 0, ({arg[0]}));
}
