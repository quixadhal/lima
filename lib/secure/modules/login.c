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
string query_body_fname();
object query_body();

void set_userid(string new_name);
void set_body(object new_body);

int matches_password(string str);
void set_password(string new_password);

void save_me();
varargs void restore_me(string some_name);

void register_failure(string addr);
void report_login_failures();

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
varargs void modal_simple(function input_func, int secure);
void modal_recapture();

void begin_info_collection();

void set_privilege(mixed priv);		// from M_ACCESS
mixed unguarded(mixed priv, function fp);


//temporary new user vars
static private int n_gen = -1;

/* forward declarations */
void rcv_userid(string arg);
void rcv_new_password(string arg);


/*
** Functions to get the body set up and the user into the game.
*/

private nomask void setup(int is_new)
{
    object o;

    remove_call_out();	/* all call outs */

    if ( !is_new )
	write("\n"+read_file(MOTD_FILE));

    report_login_failures();

    o = new(query_body_fname(), query_userid());
    set_body(o);

    LAST_LOGIN_D->register_last(query_userid(), query_ip_name(this_object()));
    if ( n_gen != -1 )
        o->set_gender(n_gen);
    save_me();

    o->enter_game(is_new);
}

private nomask void rcv_try_to_boot(object who, string answer)
{
    if ( answer == "y" )
    {
	tell_object(who, "You are taken over by yourself, or something.\n");

	/* ### may not have a body in some cases */
	who->query_body()->quit();

	setup(0);
	return;
    }
    if (answer == "n" )
    {
	if ( wizardp(query_userid()) )
	{
	    setup(0);
	    return;
	}

	write("Try another time then.\n");
	destruct(this_object());
    }

    write("please type 'y' or 'n'  >");
    modal_simple((: rcv_try_to_boot, who :));
}

static nomask void test_interactives(int is_new)
{
    object * users;
    string * ids;
    int idx;
    object the_user;

    remove_call_out();	/* all call outs */
    
/* ### temp hack for now... make sure people are auto-wizzed */
#ifdef AUTO_WIZ
    unguarded(1, (: SECURE_D->create_wizard($(query_userid())) :));
    
    /* ### auto-admin the first wizard if there are no admins */
    {
	string * members = SECURE_D->query_domain_members("admin");
	
	if ( !sizeof(members) )
	{
	    unguarded(1, (: SECURE_D->add_domain_member("admin",
							$(query_userid()),
							1) :));
	}
    }
#endif

    /* adjust the privilege of the user ob */
    if ( adminp(query_userid()) )
	set_privilege(1);
    else
	set_privilege(query_userid());

    /* check for link-deadedness */
    users = children(USER_OB) - ({ this_object() });
    ids = users->query_userid();
    if ( (idx = member_array(query_userid(), ids)) == -1 )
    {
	setup(is_new);
	return;
    }
    if ( !interactive(the_user = users[idx]) )
    {
	object body;

	/* ### the_user might not have a body yet */
	set_body(body = the_user->query_body());
	destruct(the_user);
	body->reconnect();
	return;
    }

    write("\nYou are already logged in!\nThrow yourself off?  ");
    modal_simple((: rcv_try_to_boot, the_user :));
}



/*
** Name and password processing
*/

private nomask void time_up()
{
    write("\nSorry, you've taken too long.\n");
    destruct(this_object());
}

private nomask int valid_name(string str)
{
    int len;

    if(BANISH_D->check_name(str)){
	write("Sorry, that name is forbidden by the implementors.  Please choose another.\n> ");
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

    set_password(second_entry);

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

    restore_me(arg);

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
