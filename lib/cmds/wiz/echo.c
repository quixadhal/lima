/* Do not remove the headers from this file! see /USAGE for more info. */

// See echoall header. P-)

//:COMMAND
//$$ see: echoto, echoall, echom
//USAGE:  echo <string>
//
//This command will echo a message to the room exactly as you typed the string.
//
//echo You suddenly have a strange urge to kill something.
//
//All users in the room will see the following message on their screens;
//You suddenly have a strange urge to kill something.
//
//
//***********  WARNING  ************
//
//The admin of this mud will not tolerate any type of toying
//with players. (i.e. no faked deaths or any other messages of that nature).
//Doing this is a direct violation of the mud policy and is grounds for
//disciplinary action.

inherit CMD;

private void main( string str)
{
  if ( !str )
  {
    out("Echo what?\n");
    return;
  }

  if(!end_of_pipeline())
  {
    out(str);
    return;
  }

  tell_environment(this_body(), str + "\n", 0, ({ this_body() }));
  write("You echo: "+str+"\n");
}
