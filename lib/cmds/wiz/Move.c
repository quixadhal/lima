/* Do not remove the headers from this file! see /USAGE for more info. */

//Megaboz@ZorkMUD

//:COMMAND
//USAGE:  Move <object> <object>
//
//This command will move the first object into the second one,
//both objects must be present in your environment.
//
//Move barney safe
//
//  This will put barney into the safe.

inherit CMD;

private void main( mixed *arg )
{
//### The room where the object was probably needs a message too
//### And some error-checking to ensure the move succeeds would be good
  arg[0]->move(arg[1]);
  tell(arg[0], "You are being moved to " + arg[1]->short() + "\n");
  tell_from_inside(arg[1], arg[0]->a_short() + " has been magically transported here.\n", 0, ({arg[0]}));
}
