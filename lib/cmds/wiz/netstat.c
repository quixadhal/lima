/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//USAGE: netstat
//
//Displays a list of sockets with (stat) info on each one.

inherit CMD;

private void main()
{
  object * sockets;

  sockets = children(SOCKET);
  if( !sockets || sockets == ({ }) )
  {
      out("No sockets active.\n");
  } else {
    for ( int i = 0; i < sizeof(sockets); i++ )
      out(sockets[i]->stat_me());
  }
}
