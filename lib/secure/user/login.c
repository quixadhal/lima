/* Do not remove the headers from this file! see /USAGE for more info. */

//
// Underwent a mighty transformation at the hands of Deathblade on
//	Apr 30, 1995 to move to new user/body system.
//
// written by Rust Jan 12, 1994
// su was the 13th or 14th    Extended and rewritten into the body system
// 2-5 
// stats were done the 29th.

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

void setup(int is_new);
void test_interactives(int is_new);

mixed unguarded(mixed priv, function fp);

/*
** The user's password (duh!)
*/
private string password;

//temporary new user vars
static private int n_gen = -1;

/* forward declarations */
void rcv_userid(string arg);
void rcv_new_password(string arg);


//### wah! get rid of this. need by sw_body.c
static nomask int query_n_gen() { return n_gen; }


/*
** Name and password processing
*/

nomask int matches_password(string str)
{
    return crypt(str, str) == password;
}

nomask void set_password(string str)
{
    if ( previous_object() != find_object(CMD_OB_PASSWD) )
	error("illegal attempt to set a password\n");

    password = crypt(str, str);
    save_me();
}

private nomask void time_up()
{
    write("\nSorry, you've taken too long.\n");
    destruct(this_object());
}

private nomask int valid_name(string str)
{
    int len;

    if(BANISH_D->check_name(str)){
	write("Sorry, that name is forbidden by the implementors.  Please choose another.\n");
	return 0;
    }

    if(BANISH_D->check_site()){
	printf(
	  "Sorry, your site has been banished from " + mud_name() + " To ask for a \n"
	  "Character, please mail %s\n",ADMIN_EMAIL);
	remove_call_out();
	destruct(this_object());
	return;
    }

    len = strlen(str);
    if(len>12){
	write("Sorry, that name's too long.  Try again.\n> ");
	return 0;
    }
    while(len--)
	if((str[len] < 'a' || str[len] > 'z') && 
	  (len && str[len] != '-')){
	    write(
	      "You can only use lower-case letters, or a dash if it doesn't start your name.\nTry again.\n> ");
	    return 0;
	}
    return 1;
}

private nomask void rcv_gender(string arg)
{
    arg = lower_case(arg);
    if ( arg == "f" || arg == "female")
	n_gen = 2;
    else if (arg != "m" && arg != "male")
    {
	write("I've never heard of that gender.  Please try again.\n");
	return;
    }
    else
	n_gen = 1;

    /*
    ** Done with the login sequence.  Pop our input handler now.
    */
    modal_pop();

    /*
    ** Time  go get some "user" information.
    */
    begin_info_collection();
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

    modal_func((: rcv_gender :), "Are you male or female? ");
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
	remove_call_out();
	destruct(this_object());
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

	test_interactives(0);
	return;
    }

    register_failure(query_ip_name(this_object()));
    if ( fails == 2 )
    {
	write("\nYou're just too much for me.\nSorry.\n");

	remove_call_out();
	destruct(this_object());
	return;
    }

    write("\nHmmm.....\nI'll give you another chance.\n");

    modal_func((: rcv_password, fails + 1 :), "Password: ", 1);
}

private nomask void rcv_userid(string arg)
{
    if ( !arg || arg == "" )
    {
	write("Sorry, everybody needs a name here.  Please try again.\n");
	return;
    }

    arg = lower_case(arg);

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

    /* restore the object, without worrying about preserving variables */
    restore_me(arg, 0);

    if ( arg == "guest" )
    {
	setup(0);
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
    printf("%s is running %s v. %s on %s\n\n",
	   mud_name(), lib_name(), lib_version(), driver_version());

#ifdef ZORKMUD
    write("Hello, Zorker!\n");
#else
    write("Hello, Player!\n");
#endif

    modal_push((: rcv_userid :), "Your name? ");

    /* do this to kick off the modal system and print a prompt */
    modal_recapture();
}
