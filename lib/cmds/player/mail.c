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

//:PLAYERCOMMAND
//USAGE:  mail
//        mail <user>
//        mail (<group>)
//
//Just type "mail" from anywhere in the mud, or type "mail foo" to send
//mail to someone named 'foo'.  To send mail to a group, type "mail (foo)", where 'foo' is the name of the group you want //to send to.

inherit CMD;

nomask private void main(string arg)
{
  this_user()->query_mailer()->begin_mail(arg);
}
