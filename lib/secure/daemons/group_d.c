/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** group_d.c
**
** Daemon for handling system groups.
**
** 18-Dec-94. Created. Deathblade.
*/

#include <mudlib.h>


static private mapping group_map;

void create()
{
// It might be a good idea to leave rust, beek and deathblade here until
// you really know your way around the lib, in case you run into any
// problems.  But definitly replace admin with any people that you want
// to be running the mud.
    group_map = ([
		 "admin" : ({
		     "admin",
		     "rust",
		     "beek",
		     "deathblade",
		 }),

		 "moderators" : ({
		 }),
		 ]);
}

nomask int member_group(string name, string group)
{
    return member_array(name,group_map[group])!=-1;
}

nomask mixed get_group(string group)
{
    mixed		res;

    if(!stringp(group)) return 0;
    if(group[0] == '(' && group[<1] == ')')
	group = group[1..<2];
    group = lower_case( group );
    if ( !(res = group_map[group]) )
    {
	if ( this_body() &&
	    (res = (string)this_body()->query_perm("groups")) )
	{
	    return res[group] ? res[group] : group;
	}
    }

    return res ? res : group;
}

nomask string * process_list(string * name_list)
{
    return decompose(map_array(name_list, (: get_group :)));
}


nomask mixed get_group_data()
{
    return copy(group_map);
}

nomask int adminp(object o)
{
    if ( !objectp(o) )
	return 0;

    return member_array((string)o->query_real_name(),
			group_map["admin"]) != -1;
}
