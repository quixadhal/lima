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
