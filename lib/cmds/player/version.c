/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** version.c
**
** Simple command to report various pieces of (version) data.
**
** 960506, Satan: created
*/

#include <mudlib.h>

inherit CMD;

private void main()
{
    outf("Mud Name:  %s\n", mud_name());
/* DO NOT change this; see comments in /secure/user/login.c */
    outf("Mud Lib:   %s\n", "Lima Mudlib, version 1.0a9");
    outf("Driver:    %s\n", __VERSION__ );
    outf("Host:      %s  Port: %d\n", __HOST__, __PORT__ );
    outf("Admin:     %s\n", ADMIN_EMAIL );
}
