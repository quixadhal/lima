/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <config.h>
#include <daemons.h>
#include <security.h>

string query_userid();
string query_body_fname();
object query_body();

void set_body_fname(string new_body_fname);
void set_body(object new_body);

void save_me();

void report_login_failures();

varargs void modal_simple(function input_func, int secure);

void set_privilege(mixed priv);		// from M_ACCESS
mixed unguarded(mixed priv, function fp);

int query_n_gen();


varargs nomask void switch_body(string new_body_fname, int permanent)
{
    object new_body_ob;
    object old_body;
    object where;

    if ( previous_object() != query_body() && this_body() != query_body() )
	error("* Security violation: bad body switch attempt\n");

    old_body = query_body();
    where = environment(old_body);

    if ( permanent && new_body_fname )
    {
	set_body_fname(new_body_fname);
	save_me();
    }

    if ( !new_body_fname )
	new_body_fname = query_body_fname();

    new_body_ob = new(new_body_fname, query_userid());
    set_body(new_body_ob);

    old_body->move(VOID_ROOM);
    old_body->quit();
    if(old_body)
	catch(old_body->remove());
    report_login_failures();
    new_body_ob->init_cmd_hook();
    new_body_ob->move(where);
}


/*
** Functions to get the body set up and the user into the game.
*/

static nomask void setup(int is_new)
{
    object o;

    remove_call_out();	/* all call outs */

    if ( !is_new )
	write("\n"+read_file(MOTD_FILE));

    report_login_failures();

    o = new(query_body_fname(), query_userid());
    set_body(o);

    LAST_LOGIN_D->register_last(query_userid(), query_ip_name(this_object()));
    if ( query_n_gen() != -1 )
        o->set_gender(query_n_gen());
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
