/* Do not remove the headers from this file! see /USAGE for more info. */

#include <security.h>
#include <classes.h>

inherit M_DAEMON_DATA;

/*
** Keep the list of the available skills.
**
** NOTE: The values in this mapping are unused (set to 1)
*/
private mapping skills = ([]);

#define PRIV_REQUIRED	"Mudlib:daemons"

string * register_skill(string skill)
{
    string * parts;
    string * result;

    if ( !check_privilege(PRIV_REQUIRED) )
	error("security: illegal attempt to add a skill\n");

    if ( skills[skill] )
	return 0;

    /* create the skill and all parent skills */
    parts = explode(skill, "/");
    result = allocate(sizeof(parts));
    for ( int i = sizeof(parts); i--; )
    {
	string name = implode(parts[0..i], "/");

	if ( !skills[name] )
	{
	    result[i] = name;
	    skills[result[i]] = 1;
	}
    }

    save_me();

    return filter(result, (: $1 :));
}

string * remove_skill(string skill)
{
    string * result = ({ skill });

    if ( !check_privilege(PRIV_REQUIRED) )
	error("security: illegal attempt to remove a skill\n");

    if ( !skills[skill] )
	return 0;

    /* delete the skill */
    map_delete(skills, skill);

    /* delete all subskills: skills that start with <skill> + "/" */
    skill = skill + "/";
    foreach ( string name in keys(skills) )
	if ( strsrch(name, skill) == 0 )
	{
	    map_delete(skills, name);
	    result += ({ name });
	}

    save_me();

    return result;
}

string * query_skills()
{
    return keys(skills);
}
