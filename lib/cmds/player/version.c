/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** version.c
**
** Simple command to report various pieces of (version) data.
**
** 960506, Satan: created
*/

//:PLAYERCOMMAND
//USAGE version
//
//Shows you which version of the Lima mudlib and MudOS driver are being used,
//together with IP address and port number, and the admin email address.
//But you knew all of that already, didn't you ... ?
//And it even tells you which mud you are on, in case you are completely lost

inherit CMD;

private void main()
{
    outf("Mud Name:  %s\n", mud_name());
/* DO NOT change this; see comments in /secure/user/login.c */
    outf("Mud Lib:   %s\n", "Lima Mudlib, version " + lima_version());
    outf("Driver:    %s\n", __VERSION__ );
    outf("Host:      %s  Port: %d\n", __HOST__, __PORT__ );
    outf("Admin:     %s\n", ADMIN_EMAIL );
}
