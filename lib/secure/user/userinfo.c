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

void save_me();

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


private string 	email;
private string 	real_name;
private int 	ed_setup;
private string  url;


void set_ed_setup(int code) {
    ed_setup = code;
    save_me();
}

int query_ed_setup() {
    return ed_setup;
}

//### ACK!  should disappear. need something more secure than this
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

nomask void set_url(string new_url)
{
    if ( this_body() != query_body() )
	error("illegal attempt to set URL\n");

    url = new_url;
}

nomask string query_url()
{
  return url;
}


/*
** Character creation functions: gather user info
**
** Initiate with a call to begin_info_collection()
*/
private nomask void done_with_more()
{
    /*
    ** Okay.  This input sequence and 'more' is done.  Setup the body
    ** and go.
    */
    test_interactives(1);
}

private nomask void done_with_inputs()
{

    /*
    ** Done with this series of inputs
    */
    modal_pop();

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

    more_file(NEW_PLAYER, 0, (: done_with_more :));
}

private nomask void rcv_url(string s)
{
    url = s;
    done_with_inputs();
}

private nomask void rcv_real_name(string str)
{
    real_name = str;
    modal_func((: rcv_url :), "Your home page address (if any): ");
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

    modal_push((: rcv_email_address :), "Your email address: ");
}
