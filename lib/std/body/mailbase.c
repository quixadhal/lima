/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** mailbase.c
**
** Common mail handling functions for various mail handlers to use.
** Written by Rust (?)
** Revised by Deathblade 7-Sep-94 for additional functionality
** 950706, Deathblade: removed functionality; assumed by mailbox.c
*/

private string		default_mailer;

private static string *	valid_mailers = ({
    "/secure/obj/mailers/standard", "/secure/obj/mailers/Mh"
});

/*
** Current mail handler object (clone of default_mailer)
*/
private static object	mailer;


nomask object query_mailer()
{
    if ( !mailer )
    {
	if ( default_mailer )
	    mailer = new(default_mailer);
	else
	    mailer = new(valid_mailers[0]);
    }
    return mailer;
}

nomask string query_default_mailer()
{
    return default_mailer;
}
nomask void set_default_mailer(string new_default_mailer)
{
    default_mailer = new_default_mailer;
}

nomask void unload_mailer()
{
  if ( mailer )
    mailer->remove();
}
