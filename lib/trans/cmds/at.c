/* Do not remove the headers from this file! see /USAGE for more info. */

//:COMMAND
//$$ see: at
//USAGE at <living> <command>
//
//This command allows you to remotely perform a command
//as if you were standing next to the person.
//
//at beek tickle beek
//
//Beek will see:  Zifnab tickles you.

inherit CMD;

#define USAGE "Usage: at <living> <command>\n"

private void main( mixed *arg )
{
  object start, targ;

  start = environment(this_body());
  targ = environment(arg[0]);
  if(!targ)
  {
    out("Your target has no environment.\n");
    return;
  }
  this_body()->move( targ );
  this_user()->force_me(implode(arg[1], " "));
  if(start)
    this_body()->move( start );
}
