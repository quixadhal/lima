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
inherit "/secure/modules/userinfo";

private string		name;
private string		body_fname;

static private object	body;

nomask string query_name()
{
    return name;
}
nomask string query_userid()
{
    return name;
}
nomask string query_real_name()		/* ### should remove */
{
    return stringp(name) ? name : "login";
}
static nomask void set_userid(string new_userid)
{
    name = new_userid;
}


nomask string query_body_fname()
{
//### extreme hack, until something better is done
    if (name == "littlebeek") return "/std/race/elf.c";

    /*
    ** ### need to work out proper strategy.
    ** ### elements: select appropriate body for wiz vs. player.  allow
    ** ### for wizzes to permanent change their body until they are
    ** ### dewizzed.
    ** ### -> make use of 0 to mean default, non-zero is a wiz body
    **
    ** ### for now, hard-code one of two bodies
    */

    /* ### temp change to reset old users' fnames */
    if ( body_fname == "/std/dev" )
	body_fname = 0;

    return PLAYER;

//    return body_fname;
}
static nomask void set_body_fname(string new_body_fname)
{
    // ### see query_body_fname() discussion
//    body_fname = new_body_fname;
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
