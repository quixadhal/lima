/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: dest, clean
//USAGE: clone <object>
//
//This command clones an object into your inventory if it is
//gettable, and into your environment if it isn't gettable.

#include <move.h>

inherit CMD;

private void main( mixed *arg )
{
  object o;
  int size = sizeof( arg[0]);

  if( !size )
    arg[0] = get_user_variable( "cwf" );
  o = new(arg[0]);
  if( !o )
  {
    out("Failed to load file.\n");
    return;
  }
  if( !size && !inherits( "/std/object.c", o))
  {
    destruct(o);
    out( "clone [filename]\n");
    return;
  }
  this_body()->do_player_message("clone", o);
  if (o->get() != MOVE_OK || o->move(this_body()) != MOVE_OK)
  {
    if (o->move(environment(this_body())) != MOVE_OK)
      out("Ok. (It couldn't be moved.)\n");
    else
      out("Ok. (It is on the ground.)\n");
  } else
    out("Ok. (It is in your inventory.)\n");
  o->on_clone();
  return;
}