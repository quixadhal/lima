/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** userinfo.c -- user information
**
** 950821, Deathblade: created.
*/

#include <config.h>
#include <mudlib.h>
#include <security.h>

void test_interactives(int is_new);

object query_body();
string query_userid();

varargs void modal_push(function input_func,
			mixed prompt,
			int secure,
			function return_to_func
    );
varargs void modal_func(function input_func,
			mixed prompt,
			int secure
    );
void modal_pop();

mixed unguarded(mixed priv, function fp);


private string email;
private string real_name;
private string title;


/* ### ACK!  should disappear. need something more secure than this */
nomask string query_email()
{
    return email;
}
nomask void set_email(string new_email)
{
    if ( this_body() != query_body() )
	error("illegal attempt to set email address\n");

    email = new_email;
}

nomask void set_title(string new_title)
{
    if ( this_body() != query_body() )
	error("illegal attempt to set title\n");

    title = new_title;
}

/*
** Character creation functions: gather user info
**
** Initiate with a call to begin_info_collection()
*/
private nomask void done_with_more()
{
    /*
    ** Okay.  This input sequence is done.  Setup the body and go.
    */
    modal_pop();

    test_interactives(1);
}

private nomask void rcv_real_name(string str)
{
    real_name = str;

    /*
    ** Let's move on to introducing the character to the mud.
    */
#ifdef AUTO_WIZ
    write(">>>>>You've been granted automatic guest wizard status.<<<<<\n");

    unguarded(1, (: SECURE_D->create_wizard($(query_userid())) :));
#endif

    if ( file_size(NEW_PLAYER) <= 0 )
    {
	done_with_more();
	return;
    }

    /* ### set up continuation to done_with_more() rather return_to_func */
    new(MORE_OB)->more_file(NEW_PLAYER);
}

private nomask void rcv_email_address(string str)
{
    email = str;

    modal_func((: rcv_real_name :), "Your real name: ");
}

static nomask void begin_info_collection()
{
    write("\n"
	  "The following info is only seen by you and administrators\n"
	  "  if you prepend a # to your response.\n"
	  "\n"
	  "You cannot gain wizard status without valid responses to these questions:\n"
	);

    modal_push((: rcv_email_address :), "Your email address: ", 0,
	       (: done_with_more :));
}
