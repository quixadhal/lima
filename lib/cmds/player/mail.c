/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** mail.c
**
** Invoke the modal mail processor.  Note that the player may be set up
** for Mh (command-line).  Mh will issue the appropriate message if the
** user tries this command.
**
** 05-Nov-94. Created. Deathblade.
*/

#include <mudlib.h>
inherit CMD;

nomask private void main(string arg)
{
    this_body()->query_mailer()->begin_mail(arg);
    return;
}
