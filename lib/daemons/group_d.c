/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** group_d.c
**
** Daemon for handling system groups.
**
** 941218, Deathblade: created.
** 950813, Deathblade: removed method of specifying admins here
*/

#include <mudlib.h>
#include <security.h>

inherit M_DAEMON_DATA;

/*
** The priv needed to alter the groups.  Technically, somebody with
** Mudlib:daemons could do it (by editing this file), but we'll make
** it a bit tougher for people who want to try to do that.
*/
#define PRIV_NEEDED	"Mudlib:"

private mapping group_map = ([ ]);

nomask void add_group(string group)
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("illegal attempt to add a system group\n");

    if ( !group_map[group] )
    {
	group_map[group] = ({ });
	save_me();
    }
}
nomask void remove_group(string group)
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("illegal attempt to remove a system group\n");

    map_delete(group_map, group);
    save_me();
}

nomask void add_group_member(string group, string new_member)
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("illegal attempt to add a member to a system group\n");

    if ( !group_map[group] )
	error("group does not exist\n");

    if ( member_array(new_member, group_map[group]) == -1 )
    {
	group_map[group] += ({ new_member });
	save_me();
    }
}
nomask void remove_group_member(string group, string new_member)
{
    if ( !check_previous_privilege(PRIV_NEEDED) )
	error("illegal attempt to remove a member from a system group\n");

    if ( group_map[group] )
    {
	group_map[group] -= ({ new_member });
	save_me();
    }
}

nomask int member_group(string name, string group)
{
    return group_map[group] && member_array(name, group_map[group]) != -1;
}

nomask mixed get_group(string group)
{
    mixed res;

    if ( !stringp(group) )
	return 0;
    if ( group[0] == '(' && group[<1] == ')' )
	group = group[1..<2];
    else
      return group;
    group = lower_case( group );
    if ( !(res = group_map[group]) )
    {
	if ( this_body() && (res = this_body()->query_perm("groups")) )
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
