/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** user_d.c -- user/body information access
**
** Interface:
**
**   mixed * query_variable(string userid, string * varlist)
**
**     Return an array of variable values.  0 is returned if
**     the specified user does not exist.
**
**
**   void set_variable(string userid, string varname, mixed value)
**
**     Set a variable to some value in the specified user.  An
**     error is thrown if the user does not exist.
**
**
** The calls are not orthogonal (one queries multiple, one sets a
** single value).  This is because we do not want to error() on the
** query of a non-existent user.  If the query returned a single
** value, then there would be no way to represent "no such user."
** Further, requesting multiple values with individual requests
** would be dog slow.
**
** NOTE: On both calls, you must have privilege 1 for them to succeed.
**       This is because bind() is used to access user/body objects and
**       because the save files for the user/body are read-protected.
**       In both cases, privilege 1 is required to fulfill the request.
**
** 950823, Deathblade: created
*/

#include <mudlib.h>
#include <security.h>

inherit DAEMON;

static private string *	legal_user_query =
({
    "failures",
    "email",
    "real_name",
    "password",
    "url",
});
static private string *	legal_user_set =
({
    "failures",
    "password",
});

static private string *	legal_body_query =
({
    "nickname",
    "plan",		/* only when EVERYONE_HAS_A_PLAN */
    "wiz_position",
    "title",		/* only when USE_TITLES */
});
static private string *	legal_body_set =
({
    "plan",		/* only when EVERYONE_HAS_A_PLAN */
    "wiz_position",
});

class var_info
{
    object	ob;
    string	fname;
    string *	lines;
}

private nomask mixed query_online_object(object ob, string varname)
{
    return evaluate(bind((: fetch_variable, varname :), ob));
}

private nomask mixed set_online_object(object ob, string varname, mixed value)
{
    evaluate(bind((: store_variable, varname, value :), ob));
}

private nomask mixed query_filed_object(string * lines, string varname)
{
    lines = regexp(lines, "^" + varname + " ");
    if ( !sizeof(lines) )
	return 0;
    return restore_variable(lines[0][sizeof(varname) + 1..]);
}

nomask mixed * query_variable(string userid, string * vlist)
{
    class var_info user;
    class var_info body;
    class var_info which;
    mixed * results;
    string var;

    if ( !check_privilege(1) )
	error("insufficient privilege to query variables\n");

    results = ({ });
    
    foreach ( var in vlist )
    {
	if ( member_array(var, legal_user_query) != -1 )
	{
	    if ( !user )
	    {
		user = new(class var_info);
		user->ob = find_user(userid);
		user->fname = LINK_PATH(userid) + __SAVE_EXTENSION__;
	    }

	    which = user;
	}
	else if ( member_array(var, legal_body_query) != -1 )
	{
	    if ( !body )
	    {
		body = new(class var_info);
		body->ob = find_body(userid);
		body->fname = USER_PATH(userid) + __SAVE_EXTENSION__;
	    }

	    which = body;
	}
	else
	    error("illegal variable request\n");

	if ( which->ob )
	{
	    results += ({ query_online_object(which->ob, var) });
	}
	else
	{
	    if ( !which->lines )
	    {
		if ( !is_file(which->fname) )
		{
		    /* no such player */
		    return 0;
		}

		which->lines = explode(read_file(which->fname), "\n");
	    }

	    results += ({ query_filed_object(which->lines, var) });
	}
    }

    return results;
}

nomask void set_variable(string userid, string varname, mixed value)
{
    string fname;
    object ob;
    mixed * lines;

    if ( !check_privilege(1) )
	error("insufficient privilege to set variables\n");

    if ( member_array(varname, legal_user_set) != -1 )
    {
	fname = LINK_PATH(userid);
	ob = find_user(userid);
    }
    else if ( member_array(varname, legal_body_set) != -1 )
    {
	fname = USER_PATH(userid);
	ob = find_body(userid);
    }

    if ( !fname )
	error("illegal variable assignment\n");

    if ( ob )
	return set_online_object(ob, varname, value);

    fname += __SAVE_EXTENSION__;
    if ( !is_file(fname) )
	error("no such user\n");

    lines = regexp(explode(read_file(fname), "\n"),
		  "^" + varname + " ",
		   2);
    write_file(fname, implode(lines, "\n") +
	       sprintf("\n%s %s\n", varname, save_variable(value)),
	       1);
}
