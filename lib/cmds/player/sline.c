/* Do not remove the headers from this file! see /USAGE for more info. */


#ifdef USE_STATUS_LINE
//:PLAYERCOMMAND
//USAGE sline
//      sline on|off
//
//Shows whether the status line is on or off (ie whether it will be displayed
//on the screen), and allows you to turn it on/off as desired.
//If the mud has decided not to use the status line system,
//this will probably have no effect.

inherit CMD;

private void main( string arg )
{
  if ( !arg )
  {
    if ( get_user_variable("status") )
      arg = "off";
    else
       arg = "on";
  }

  switch ( arg )
  {
    case "on":
	this_user()->query_shell_ob()->set_variable("status", 1);
	this_body()->update_status_line();
	break;

    case "off":
	this_user()->query_shell_ob()->unset_variable("status");
	this_body()->remove_status_line();
	break;

    default:
	out("Use \"sline on\", \"sline off\", or just \"sline\" to toggle.\n");
  }
}
#endif
