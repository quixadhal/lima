/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** user_d.c -- user/body information access
**
** 950823, Deathblade: created
*/

#include <mudlib.h>
#include <security.h>

inherit DAEMON;

static private string *	legal_user_query =
({
    "failures",
});
static private string *	legal_user_set =
({
    "failures",
});

static private string *	legal_body_query =
({
    "nickname",
});
static private string *	legal_body_set =
({
});

private nomask mixed query_online_object(object ob, string varname)
{
    return evaluate(bind((: fetch_variable, varname :), ob));
}

private nomask mixed set_online_object(object ob, string varname, mixed value)
{
    evaluate(bind((: store_variable, varname, value :), ob));
}

nomask mixed query_variable(string userid, string varname)
{
    string fname;
    object ob;
    mixed * lines;

    if ( member_array(varname, legal_user_query) != -1 )
    {
	fname = LINK_PATH(userid);
	ob = find_user(userid);
    }
    else if ( member_array(varname, legal_body_query) != -1 )
    {
	fname = USER_PATH(userid);
	ob = find_body(userid);
    }

    if ( !fname )
	error("illegal variable request\n");

    if ( ob )
	return query_online_object(ob, varname);

    lines = regexp(explode(read_file(fname + __SAVE_EXTENSION__), "\n"),
		  "^" + varname + " ");
    return restore_variable(lines[0][member_array(' ', lines[0]) + 1..]);
}

nomask void set_variable(string userid, string varname, mixed value)
{
    string fname;
    object ob;
    mixed * lines;

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
	return query_online_object(ob, varname);

    fname += __SAVE_EXTENSION__;
    lines = regexp(explode(read_file(fname), "\n"),
		  "^" + varname + " ",
		   2);
    write_file(fname, implode(lines, "\n") +
	       sprintf("\n%s %s\n", varname, save_variable(value)),
	       1);
}
