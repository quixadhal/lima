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

void userinfo_handle_logon();
void sw_body_handle_existing_logon(int);

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

mixed unguarded(mixed priv, function fp);

/* Login states */
#define TIMEOUT			-1
#define INITIAL_PROMPT		0
#define NAME_PROMPT		1
#define CONFIRM_NEW_NAME	2
#define NEW_PASSWORD		3
#define CONFIRM_PASSWORD	4
#define GET_PASSWORD		10

/*
** The user's password (duh!)
*/
//### this has a bad salt in it (the password itself)... this should be fixed
private string password;

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
//### I believe it is; MudOS is actually fairly tolerant of desting people
//### from arbitrary routines
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

    switch ( arg )
    {
    case "who":
	b = bodies()->query_name();
	b -= ({ "Someone" });
	b-= ({ });
b-= ({ 0 });
	switch ( sizeof(b) )
	{
	case 0:
	    write("No one appears to be logged on.\n");
	    break;

	case 1:
	    printf( "Only %s is currently on.\n", b[0]);
	    break;

	default:
	    printf("The following people are logged on:\n%s\n",
		   implode(b,", "));
	    break;
	}
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

private nomask void modify_guest_userid()
{
    string array userids = users()->query_userid();

    for ( int i = 1; ; ++i )
	if ( member_array("guest" + i, userids) == -1 )
	{
	    set_userid("guest" + i);
	    save_me();
	    return;
	}
}

/*
 * Handles most of Phase 0 of the login process.  At this point, we have
 * neither a shell nor a body.
 *
 * Note that this routine is written in continuation passing style.  If
 * you're not familiar with it, don't worry; the idea is quite simple.
 *
 * Though LPC doesn't support threads, it does support including function
 * arguments as parameters.  In this case, we use one of the function
 * parameters ('state') to include information about where we are in
 * the process, and use a switch() statement to return to where we left
 * off when the driver returns information to us.  This allows this to
 * be written as one routine, instead of a series of smaller routines.
 * every 'break;' statement corresponds to waiting for information from
 * the user, and what we are waiting for is commented.  The exception
 * is the time_out timer, which transfers us to state -1 when it goes
 * off.
 *
 * This had several advantages; first, it removes the need for forward
 * declarations; secondly, it allows you to follow the login process
 * simply by reading this routine in order, instead of trying to figure
 * out which routine calls which routine.
 */
private nomask varargs
void login_handle_logon(int state, mixed extra, string arg) {
    switch (state) {
    case INITIAL_PROMPT:
	/* setup timeout */
	call_out((: login_handle_logon, TIMEOUT :), LOGIN_NAME_WAIT);

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
	printf("%s is running Lima 1.0a5 on %s\n\n",
	       mud_name(), driver_version());
	
#ifdef ZORKMUD
	write("Hello, Zorker!\n");
#else
	write("Hello, Player!\n");
#endif

	modal_push((: login_handle_logon, NAME_PROMPT, 0 :), LOGIN_PROMPT);
	
	/* do this to kick off the modal system and print a prompt */
	modal_recapture();
	break;

	/******************* NAME PROMPT **********************/
    case NAME_PROMPT:
	if ( !arg || arg == "" ) {
	    write("Sorry, everybody needs a name here.  Please try again.\n");
	    return;
	}

	arg = lower_case(arg);
	if ( !check_special_commands(arg) )
	    return;

	if ( unguarded(1, (: file_size,
			   LINK_PATH(arg) + __SAVE_EXTENSION__ :)) <= 0 ) {
	    if ( !valid_name(arg) ) {
		/* return for another user id */
		return;
	    }

	    modal_func((: login_handle_logon, CONFIRM_NEW_NAME, arg :),
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

	if ( arg == "guest" ) {
	    modify_guest_userid();
	    modal_pop();
	    sw_body_handle_existing_logon(1);
	    return;
	}

	modal_func((: login_handle_logon, GET_PASSWORD, 0 :), "Password: ", 1);

	/*
	** Adjust the time we'll wait for the user
	*/
	remove_call_out();	/* all call outs */
	call_out((: login_handle_logon, -1 :), LOGIN_PASSWORD_WAIT);
	break;

	/************ IS 'NAME' CORRECT? ************/
    case CONFIRM_NEW_NAME:
	arg = lower_case(arg);
	switch ( arg ) {
	case "n":  case "no":  case "nay":
	    modal_func((: login_handle_logon, NAME_PROMPT, 0 :),
		       "Please enter your name (preferably correctly this time): ");
	    break;

	case "y":  case "yes":  case "aye":
#ifdef NO_NEW_PLAYERS
	    write("Unfortunately, "+mud_name()+" is still in the " 
		  "developmental stage, and is not accepting new users. " 
		  "If it is urgent, please use the guest character.\n");
	    get_lost();
	    return;
#endif /* NO_NEW_PLAYERS */

	    /*
	    ** Begin the character creatin sequence. Store their name.
	    */
	    set_userid(extra);

#ifdef ZORKMUD
	    write("\nAh, a New Zorker.\n");
#else
	    write("\nAh, a New Player.\n");
#endif

	    modal_func((: login_handle_logon, NEW_PASSWORD, 0 :), "Password: ", 1);
	    break;

	case "maybe":  case "possibly":  case "mu":  case "perhaps":
	    write("You can play games later. ");

	    /* FALLTHRU */
	default:
	    write("Please answer Yes or No.\n");
	    break;
	}
	break;

	/************ NEW PASSWORD *****************/
    case NEW_PASSWORD:
	if ( strlen(arg) < 5 ) {
	    write("Your password must have at least 5 characters in it.\n");
	    return;
	}

	write("\n");	/* needed after a no-echo input */

	modal_func((: login_handle_logon, CONFIRM_PASSWORD, crypt(arg, arg) :),
		   "Again to confirm: ", 1);
	break;
	
	/************ CONFIRM PASSWORD *************/
    case CONFIRM_PASSWORD:
	if ( crypt(arg, arg) != extra ) {
	    write("\nSocks don't need to, but passwords have to match.\n");

	    modal_func((: login_handle_logon, NEW_PASSWORD, 0 :), "Password: ", 1);
	    return;
	}

	password = crypt(arg, arg);

	write("\n");	/* needed after a no-echo input */

	/*
	** Done with the login sequence.  Pop our input handler now.
	*/
	modal_pop();

	/*
	** Time  go get some "user" information.
	*/
	userinfo_handle_logon();
	break;
	
	/************ PASSWORD PROMPT **************/
    case GET_PASSWORD:
	if ( matches_password(arg) ) {
	    /*
	    ** Done with the login sequence.  Pop our input handler now.
	    */
	    modal_pop();

	    sw_body_handle_existing_logon(0);
	    return;
	}

	register_failure(query_ip_name(this_object()));
	if ( extra == 2 ) {
	    write("\nYou're just too much for me.\nSorry.\n");

	    get_lost();
	    return;
	}

	write("\nHmmm.....\nI'll give you another chance.\n");

	modal_func((: login_handle_logon, GET_PASSWORD, extra + 1 :), "Password: ", 1);
	break;

    case TIMEOUT:   /* The timer has expired */
	write("\nSorry, you've taken too long.\n");
	get_lost();
	break;
    }
}

/*
** logon()
**
** Invoked by the driver to set up the object for logging in
*/
private nomask void logon()
{
    login_handle_logon(INITIAL_PROMPT, 0);
}
