/* Do not remove the headers from this file! see /USAGE for more info. */

#include <security.h>
#include <classes.h>

#define SKILL_SAVE_FILE "/data/daemons/skill_d"

inherit M_ACCESS;

/*
** Keep the list of the available skills.
**
** NOTE: The values in this mapping are unused (set to 1)
*/
private mapping skills = ([]);

#define PRIV_REQUIRED	"Mudlib:daemons"


private void save_me()
{
    unguarded(1, (: save_object, SKILL_SAVE_FILE :));
}

private void create()
{
    set_privilege(1);
    restore_object(SKILL_SAVE_FILE);
}

void register_skill(string skill)
{
    if ( !check_privilege(PRIV_REQUIRED) )
	error("security: illegal attempt to add a skill\n");

    if ( skills[skill] )
	return;

    skills[skill] = 1;
    save_me();
}

void remove_skill(string skill)
{
    if ( !check_privilege(PRIV_REQUIRED) )
	error("security: illegal attempt to remove a skill\n");

    if ( !skills[skill] )
	return;

    map_delete(skills, skill);
    save_me();
}

string * query_skills()
{
    return keys(skills);
}
