/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** user.c -- a user object
**
** This object represents a particular user and that user's connection.
**
** 95-Apr-30.  Deathblade.  Created.
*/

#include <mudlib.h>
#include <config.h>
#include <security.h>

inherit M_ACCESS;

inherit "/secure/modules/login";
inherit "/secure/modules/sw_body";
inherit "/secure/modules/sw_user";
inherit "/secure/modules/loginfail";
inherit "/secure/modules/inputsys";

private string		name;
private string		password;

private string		body_fname = PLAYER;	/* ### should fix name */

static private object	body;

nomask string query_name()
{
    return name;
}
nomask string query_userid()
{
    return name;
}
static nomask void set_userid(string new_userid)
{
    name = new_userid;
}

nomask int matches_password(string str)
{
    return crypt(str, str) == password;
}
static nomask void set_password(string str)
{
    password = crypt(str, str);
}

nomask string query_body_fname()
{
    return body_fname;
}
static nomask void set_body_fname(string new_body_fname)
{
    body_fname = new_body_fname;
}

nomask object query_body()
{
    return body;
}
static nomask void set_body(object new_body)
{
    body = new_body;
}

void remove()
{
    if ( !body || previous_object() == body )
    {
	destruct(this_object());
    }
}

/* ### what is this for?  this object shouldn't clean up, ever...
int clean_up()
{
    return body == 0;
}
*/

static nomask void save_me()
{
    unguarded(1, (: save_object, LINK_PATH(name) :));
}
varargs static nomask void restore_me(string some_name, int flag)
{
    if ( !some_name )
	some_name = name;
    unguarded(1, (: restore_object, LINK_PATH(some_name), flag :));
}

private nomask void net_dead()
{
    /*
    ** Tell the body about the net death.  The user object should stick
    ** around, though, so we can find it again if the user reconnects.
    ** If there is no body yet, then just torch self.
    */
    if ( body )
	body->net_dead();
    else
	destruct(this_object());
}
