/* Do not remove the headers from this file! see /USAGE for more info. */

// Looks like Rust.  Who couldn't write this in 2 seconds, tho?

//:COMMAND
//$$ see: echo, echoto. echom
//USAGE:  echoall <string>
//
//This command will echo a message to the entire mud exactly as you
//entered it.
//
//
//echoall You suddenly have a strange urge to kill something.
//
//All the users on the mud will see on their screen;
//You suddenly have this urge to kill something.
//
//
//***********  WARNING  ************
//
//The admin of this mud will not tolerate any type of toying
//with players.  (i.e. no faked deaths, or messages of that nature).
//Doing this is a direct violation of the mud policy and is grounds for
//disciplinary action.

inherit CMD;

void create()
{
  ::create();
  no_redirection();
}

private void main(string orig_input, mixed * arg)
{
  string msg = implode(arg[0], " ");
  out("You echoall: " + msg + "\n");
  tell(users() - ({ this_user() }), msg + "\n");
}
