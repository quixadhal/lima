/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: echo, echoall, echom
//USAGE: echoto <target> <message>
//
//Provides a simple targetted echo facility,
//
//>echoto fred This is a message from echoto.
//
//Fred sees:
//  This is a message from echoto.
//Nobody else in the room sees anything
//except you see:
//  You echo to Fred: This is a message from echoto.

inherit CMD;

void create()
{
  ::create();
  no_redirection();
}

private void main( string orig_input, mixed arg, mapping flags)
{
  string msg = orig_input[strsrch(orig_input," ")+1..];
  outf("You echo to %s: %s\n", arg[0]->query_name(), msg);
  tell(arg[0], msg + "\n");
}
