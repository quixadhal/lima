/* Do not remove the headers from this file! see /USAGE for more info. */

//
// written by Rust Jan 12, 1994
// Underwent a mighty transformation at the hands of Deathblade on
//	Apr 30, 1995 to move to new user/body system.
//


#include <mudlib.h>
#include <daemons.h>
#include <config.h>
#include <commands.h>
#include <security.h>

string query_userid();
void set_userid(string new_name);

void save_me();
void restore_me(string some_name, int preserve_vars);

void register_failure(string addr);

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
void modal_recapture();

void begin_info_collection();

void existing_user_enter_game();
void existing_user_ready();

mixed unguarded(mixed priv, function fp);

/*
** The user's password (duh!)
*/
private string password;

/* forward declarations */
void rcv_userid(string arg);
void rcv_new_password(string arg);



private nomask void get_lost_now()
{
    destruct();
}

/*
** This is used when we want to get rid of somebody... we have to wait a
** bit for MudOS to flush the buffer before disconnecting them (desting
** this_object()).
*/
//### if MudOS waits before closing the socket, this system may be obsolete
private nomask void get_lost()
{
    remove_call_out();
    modal_func((: 1 :), "");	/* ignore all input */
    call_out((: get_lost_now :), 2);
}

/*
** Name and password processing
*/

nomask int matches_password(string str)
{
    return crypt(str, str) == password;
}

nomask void set_password(string str)
{
    if ( base_name(previous_object()) != CMD_OB_PASSWD )
	error("illegal attempt to set a password\n");

    password = crypt(str, str);
    save_me();
}

private nomask void time_up()
{
    write("\nSorry, you've taken too long.\n");
    get_lost();
}

private nomask int check_site()
{
    if ( BANISH_D->check_site() )
    {
	printf("Sorry, your site has been banished from " + mud_name() + ".  To ask for\n"
	       "a character, please mail %s.\n",
	       ADMIN_EMAIL);

	get_lost();

	return 0;
    }

    return 1;
}

private nomask int valid_name(string str)
{
    int len;

    if ( BANISH_D->check_name(str) )
    {
	write("Sorry, that name is forbidden by the implementors.  Please choose another.\n");
	return 0;
    }

    if ( !check_site() )
	return 0;

    len = strlen(str);
    if ( len > 12 )
    {
	write("Sorry, that name's too long.  Try again.\n> ");
	return 0;
    }

    /*
    ** We used to rely on the banish code to do this, but that is a
    ** Bad Thing as it is too easy to see those entries deleted.  In
    ** particular, we've been distributing the lib for a while now
    ** without a banish.o that contains this rule.  To use the banish
    ** code to implement a feature here is too convoluted and unintuitive.
    **
    ** So... we'll put a regex here to check the name.
    **
    ** Note that this regex matches the restriction imposed by the
    ** SECURE_D.  Also note the name is in lower case right now.
    */
    if ( !regexp(str, "^[a-z]+$") )
    {
	write("Sorry, that name is forbidden by the implementors.  Please\n"
	      "choose a name containing only letters.\n");
	return 0;
    }

    return 1;
}


private nomask int check_special_commands(string arg)
{
    string array b;

    switch(arg)
    {
    case "who":
	b = bodies()->query_name();
	if(!sizeof(b))
	{
	    write("No one appears to be logged on.\n");
	    return 0;
	}
	printf("The following people are logged on:\n%s\n",
	       wrap(implode(b,", ")));
	return 0;

    case "":
    case "quit":
    case "exit":
    case "leave":
	write("Bye.\n");
	get_lost();
	return 0;

    default:
	return 1;
    }
}  

private nomask void rcv_confirm_new_pass(string first_entry,
					 string second_entry)
{
    if ( crypt(second_entry, second_entry) != first_entry )
    {
	write("\nSocks don't need to, but passwords have to match.\n");

	modal_func((: rcv_new_password :), "Password: ", 1);
	return;
    }

    password = crypt(second_entry, second_entry);

    write("\n");	/* needed after a no-echo input */

#ifdef CONF_IN_PROGRESS
    CONF_D->add_visitor(name);
#endif

    /*
    ** Done with the login sequence.  Pop our input handler now.
    */
    modal_pop();

    /*
    ** Time  go get some "user" information.
    */
    begin_info_collection();
}

private nomask void rcv_new_password(string arg)
{
    if ( strlen(arg) < 5 )
    {
	write("Your password must have at least 5 characters in it.\n");
	return;
    }

    write("\n");	/* needed after a no-echo input */

    modal_func((: rcv_confirm_new_pass, crypt(arg, arg) :),
	       "Again to confirm: ", 1);
}

private nomask void rcv_check_new_user(string the_userid, string str)
{
    str = lower_case(str);
    switch ( str )
    {
    case "n":  case "no":  case "nay":
	modal_func((: rcv_userid :),
		   "Please enter your name (preferably correctly this time): ");
	break;

    case "y":  case "yes":  case "aye":
#ifdef NO_NEW_PLAYERS
	write(wrap("Unfortunately, "+mud_name()+" is still in the " 
		   "developmental stage, and is not accepting new users. " 
		   "If it is urgent, please use the guest character.")+"\n");
	get_lost();
	return;
#endif /* NO_NEW_PLAYERS */

	/*
	** Begin the character creatin sequence. Store their name.
	*/
	set_userid(the_userid);

#ifdef ZORKMUD
	write("\nAh, a New Zorker.\n");
#else
	write("\nAh, a New Player.\n");
#endif

	modal_func((: rcv_new_password :), "Password: ", 1);
	break;

    case "maybe":  case "possibly":  case "mu":  case "perhaps":
	write("You can play games later. ");

	/* FALLTHRU */

    default:
	write("Please answer Yes or No.\n");
	break;
    }
}

private nomask void rcv_password(int fails, string arg)
{
    if ( matches_password(arg) )
    {
	/*
	** Done with the login sequence.  Pop our input handler now.
	*/
	modal_pop();

	existing_user_ready();
	return;
    }

    register_failure(query_ip_name(this_object()));
    if ( fails == 2 )
    {
	write("\nYou're just too much for me.\nSorry.\n");

	get_lost();
	return;
    }

    write("\nHmmm.....\nI'll give you another chance.\n");

    modal_func((: rcv_password, fails + 1 :), "Password: ", 1);
}

private nomask void modify_guest_userid()
{
    string * userids = users()->query_userid();

    for ( int i = 1; ; ++i )
	if ( member_array("guest" + i, userids) == -1 )
	{
	    set_userid("guest" + i);
	    save_me();
	    return;
	}
}

private nomask void rcv_userid(string arg)
{
    if ( !arg || arg == "" )
    {
	write("Sorry, everybody needs a name here.  Please try again.\n");
	return;
    }

    arg = lower_case(arg);
    if ( !check_special_commands(arg) )
    {
	return;
    }

    if ( unguarded(1, (: file_size,
		       LINK_PATH(arg) + __SAVE_EXTENSION__ :)) <= 0 )
    {
	if ( !valid_name(arg) )
	{
	    /* return for another user id */
	    return;
	}

	modal_func((: rcv_check_new_user, arg :),
		   "Is '" + capitalize(arg) + "' correct? ");
	return;
    }

    /* always check the site */
    if ( arg == "guest" && !check_site() )
	return;

    /*
    ** Restore the object, without worrying about preserving variables.
    ** Note that this sets the userid value.
    */
    restore_me(arg, 0);

    if ( arg == "guest" )
    {
	modify_guest_userid();
	existing_user_enter_game();
	return;
    }

    modal_func((: rcv_password, 0 :), "Password: ", 1);

    /*
    ** Adjust the time we'll wait for the user
    */
    remove_call_out();	/* all call outs */
    call_out((: time_up :), LOGIN_PASSWORD_WAIT);
}


/*
** logon()
**
** Invoked by the driver to set up the object for logging in
*/
private nomask void logon()
{
    call_out((: time_up :), LOGIN_NAME_WAIT);

    write("");
    write(read_file(WELCOME_FILE));
/*
 * Warning: We have put literally thousands of hours of work into this
 * mudlib, and given it to you for free, and all we ask is that you give
 * us credit by leaving the lib version alone and visible on the login
 * screen.  Is that really so much to ask?
 *
 * If you think you have changed the lib to the point where you should
 * be allowed to change this, ask us first.  Please make sure you have
 * extensively modified/rewritten more than half of the base mudlib first
 * (intend to modify ... doesn't cut it)
 */
    printf("%s is running Lima 0.9r12 (pre-alpha) on %s\n\n",
	   mud_name(), driver_version());

#ifdef ZORKMUD
    write("Hello, Zorker!\n");
#else
    write("Hello, Player!\n");
#endif

    modal_push((: rcv_userid :), LOGIN_PROMPT);

    /* do this to kick off the modal system and print a prompt */
    modal_recapture();
}
