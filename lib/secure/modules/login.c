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
#include <security.h>
#include <config.h>
#include <commands.h>



string query_userid();
string query_body_fname();
object query_body();

void set_userid(string new_name);
void set_body_fname(string new_body_fname);
void set_body(object new_body);

int matches_password(string str);
void set_password(string new_password);

void save_me();
varargs void restore_me(string some_name);

void register_failure(string addr);
void report_login_failures();


void set_privilege(mixed priv);		// from M_ACCESS


private string email;
private string real_name;
private int level;
private string title;
private int newwiz;

//temporary new user vars
static private int n_gen = -1;


private nomask void logon()
{
    call_out("time_up", 300);
    write("");
    write(read_file(WELCOME_FILE));
    printf("%s is running %s v. %s on %s\n\n",
	   mud_name(), lib_name(), lib_version(), driver_version());
#ifdef ZORKMUD
    write("Hello, Zorker!\nYour name? ");
#else
    write("Hello, Player!\nYour name? ");
#endif
    input_to("get_name");
}

private nomask void setup(int is_new)
{
    object o;

    remove_call_out("time_up");
#ifndef AUTO_WIZ
    if ( SECURE_D->query_is_wizard(query_userid()) )
    {
	set_body_fname(DEV);
	enable_wizard();
    }
    else
    {
	set_body_fname(PLAYER);
    }
#else
    set_body_fname(DEV);
    enable_wizard();
#endif

    if ( !is_new )
	write("\n"+read_file(MOTD_FILE));
    else level = 1;

    o = clone_object(query_body_fname());

    report_login_failures();
    o->init_user(query_userid());
    LAST_LOGIN_D->register_last(query_userid(), query_ip_name(this_object()));
if ( n_gen != -1 )
        o->set_gender(n_gen);
    save_me();

    set_body(o);
    if(newwiz)
      o->enter_game(newwiz); 
    else
      o->enter_game(is_new);
}

private nomask void test_interactives()
{
    object * users;
    string * ids;
    int idx;
    object the_user;

    remove_call_out("time_up");
    
    if(level) enable_wizard();
    /* adjust the privilege of the user ob */
    if ( GROUP_D->adminp(this_object()) )
	set_privilege(1);
    else
	set_privilege(query_userid());

    /* check for link-deadedness */
    users = children(USER_OB) - ({ this_object() });
    ids = users->query_userid();
    if ( (idx = member_array(query_userid(), ids)) == -1 )
    {
	setup(0);
	return;
    }
    if ( !interactive(the_user = users[idx]) )
    {
	object body;

	set_body(body = the_user->query_body());
	destruct(the_user);
	body->reconnect();
	return;
    }


    write("\nYou are already logged in!\nThrow yourself off?  ");
    input_to("try_to_boot", 0, the_user);
}

private nomask void check_password(string arg, int fails)
{
    if ( matches_password(arg) )
	return test_interactives();

    register_failure(query_ip_number(this_object()));
    if(fails == 2){
	write("\nYou're just too much for me.\nSorry.\n");
	destruct(this_object());
    }
    fails++;
    write("Hmmm.....\nI'll give you another chance.\nYour Password: ");
    input_to("check_password",1,fails);
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
	destruct(this_object());
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

private nomask void get_name(string arg)
{
    if(!arg || arg == ""){
	write("Sorry, everybody needs a name here.\nTry again: ");
	input_to("get_name");
	return;
    }
    arg = lower_case(arg);
    restore_me(arg);
    if ( query_userid() != arg)
    {
	if(!valid_name(arg)){
	    input_to("get_name");
	    return 0;
	}
	write ("Is '" +capitalize(arg)+ "' correct? ");
	input_to("check_new");

	set_userid(arg);
	return;
    }
    if(arg == "guest") return setup(0);
    write("Password: ");
    input_to("check_password",1,0);
    remove_call_out("time_up");
    call_out("time_up",180);
}

private nomask void impress_new_users(string foo, int i)
{
    string msg;
    if (msg = read_file(NEW_PLAYER + "." + i))
    {
	write(msg);
	write("[MORE]");
	input_to("impress_new_users", 0, i+1);
    } else {
	setup(1);
    }
}


private nomask int check_new(string str)
{
    str = lower_case(str);
    switch(str)
    {
    case "n":  case "no":  case "nay":
    {
	write( "Please enter your name (preferably correctly this time): ");
	input_to("get_name");
	return 1;
    }
    case "y":  case "yes":  case "aye":
    {
#ifdef NO_NEW_PLAYERS
	write(wrap("Unfortunately, "+mud_name()+" is still in the " 
		   "developmental stage, and is not accepting new users. " 
		   "If it is urgent, please use the guest character.")+"\n"); 
	destruct(this_object()); 
#endif /* NO_NEW_PLAYERS */ 
#ifdef ZORKMUD
	write("\nAh, a New Zorker.\nPassword: "); 
#else
	write("\nAh, a New Player.\nPassword: "); 
#endif
	input_to("get_new_password",1); 
	return 1;
    }
    case "maybe":  case "possibly":  case "mu":  case "perhaps":
    {
	write("You can play games later. ");
    }
    default:
    {
	write("(Perhaps you'd like to answer Yes or No): ");
	input_to("check_new");
	return 1;
    }
    }
}

nomask void set(string key, mixed value)
{
    switch ( key )
    {
    case "newwiz":
      if(previous_object() == query_body())
	newwiz = 1;
      break;
    case "title":
	if(previous_object() == query_body())
	    title = value;
	break;
    case "body":
	if(previous_object() == query_body())
	    set_body_fname(value);
	break;
   case "password":
        if(previous_object() == find_object(CMD_OB_PASSWD))
	  set_password(value);
	break;
    case "level":
	if(previous_object() == query_body())
	{
	    level = value;
	    if ( level )
		enable_wizard();
	    else
		disable_wizard();
	}
	break;

    case "email":
	if ( this_body() == query_body() )
	    email = value;
	break;

    default:
	error("unknown attribute\n");
    }
    save_me();
}

private nomask void try_to_boot(string answer, object who)
{
    if ( answer == "y" )
    {
	tell_object(who, "You are taken over by yourself, or something.\n");
	who->query_body()->quit();
	setup(0);
	return;
    }
    if (answer == "n" )
    {
	if( level ) return setup(0);
	write("Try another time then.\n");
	destruct(this_object());
    }
    write("please type 'y' or 'n'  >");
    input_to("try_to_boot", 0, who);
}


// New Player stuff.....


private nomask void get_new_password(string arg)
{
    if(strlen(arg) < 5)
    {
	write("Your password must have at least 5 characters in it.\nPassword: ");
	input_to("get_new_password",1);
	return;
    }
    write("\nAgain to confirm: ");
    input_to("confirm_new_pass",1,crypt(arg, arg));
}

private nomask void new_u_setup()
{
#ifdef CONF_IN_PROGRESS
    CONF_D->add_visitor(name);
#endif
    write("\nAre you male or female?  ");
    input_to("get_gender");
}

private nomask void confirm_new_pass(string sec, string fst)
{
    if(crypt(sec,sec) != fst){
	write(
	  "Socks don't need to, but passwords have to match.\n");
	write("Password:  ");
	input_to("get_new_password",1);
	return;
    }

    set_password(sec);
    new_u_setup();
}


private nomask void get_gender(string arg)
{
    if(arg == "f" || arg == "female") n_gen = 2;
    else if(arg != "m" && arg != "male"){
	write("You must choose male or female:  ");
	input_to("get_gender");
	return;
    }
    else n_gen = 1;
    write("\nThe following info is only seen by you and administrators\n"
	  "  if you prepend a # to your response.\n");
    write("\nYou cannot gain wizard status without valid responses to these questions:\n");
    write("Your e-mail address:  ");
    input_to("get_email");
}

private nomask void get_email(string str)
{
    email = str;
    write("\nYour real name:  ");
    input_to("get_real_name");
}

private nomask void get_real_name(string str)
{
    real_name = str;

#ifdef AUTO_WIZ
    write(">>>>>You've been granted automatic guest wizard status.<<<<<\n");
#endif

    /* by Beek, when he decided we need to impress new players */
    impress_new_users("", 1);
}

private nomask void time_up()
{
    write("\nSorry, you've taken too long.\n");
    destruct(this_object());
}


nomask mixed *query_who_data()
{
    return ({ 0, query_userid(), query_userid(), 0 });
}


nomask string query_real_name()
{
    return stringp(query_userid()) ? query_userid() : "login";
}

nomask string query_email()
{
    return email;
}
