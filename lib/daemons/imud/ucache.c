/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** ucache.c -- user cache information packet handling
**
** 960124, Deathblade: created
*/

class user_info
{
    string visname;
    int    gender;	/* our gender: neuter, male, female */
}

/*
** The (remote) user cache.  It maps mudnames to mappings of usernames to
** user information.
*/
nosave private mapping ucache = ([ ]);

protected nomask void add_cache_entry(string mudname, string username,
				   string visname, int gender)
{
    class user_info user;

    if ( !ucache[mudname] )
	ucache[mudname] = ([ ]);

    user = new(class user_info);
    user->visname = visname;
    user->gender  = gender;

    ucache[mudname][username] = user;
}

protected nomask void rcv_ucache_update(string orig_mud, string orig_user,
				     string target_user, mixed * message)
{
    int gender = (message[2] + 1) % 3;	/* I3 -> Lima */

    add_cache_entry(orig_mud, message[0], message[1], gender);
}

nomask string get_visname(string mudname, string username)
{
    class user_info user;

    if ( !ucache[mudname] )
	return 0;

    user = ucache[mudname][username];
    if ( !user )
	return 0;

    return user->visname;
}
nomask int get_gender(string mudname, string username)
{
    class user_info user;

    if ( !ucache[mudname] )
	return -1;

    user = ucache[mudname][username];
    if ( !user )
	return -1;

    return user->gender;
}
