/* Do not remove the headers from this file! see /USAGE for more info. */

//Megaboz@ZorkMUD

#include <mudlib.h>
inherit CMD;

private void main( mixed *arg )
{
    arg[0]->move(arg[1]);
    tell_object(arg[0], "You are being moved to " + arg[1]->short() + "\n");
    tell_object(arg[1], arg[0]->short() + " has been magically transported here.\n");
}
