/* Do not remove the headers from this file! see /USAGE for more info. */

#include <mudlib.h>
#include <config.h>
#include <daemons.h>

string query_userid();
string query_body_fname();
object query_body();

void set_body_fname(string new_body_fname);
void set_body(object new_body);

void save_me();

void report_login_failures();


nomask void switch_body(mixed new_body)
{
    object new_body_ob;
    object old_body;
    object where;
    int success;

    if ( previous_object() != query_body() && this_body() != query_body() )
	error("* Security violation: bad body switch attempt\n");

    old_body = query_body();
    where = environment(old_body);
    if ( objectp(new_body) )
	new_body_ob = new_body;
    else
    {
	if ( new_body )
	    set_body_fname(new_body);
	new_body_ob = clone_object(query_body_fname());
    }
    new_body_ob->init_user(query_userid());
    set_body(new_body_ob);
    old_body->move(VOID_ROOM);
    old_body->quit();
    if(old_body)
	catch(old_body->remove());
    report_login_failures();
    new_body_ob->init_cmd_hook();
    new_body_ob->move(where);
    save_me();
}
