/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** skills.c -- print out the player's skills
**
** 960522, Deathblade: Created.
*/

#include <mudlib.h>
#include <config.h>
#include <classes.h>

inherit CMD;
inherit CLASS_SKILL;

private void main(string arg)
{
#ifndef USE_SKILLS
    out(mud_name() + " has no player skills, sorry.\n");
#else

    class skill * skills = this_body()->get_skills();
    string name;
    class skill skill;

    if ( sizeof(skills) == 0 )
    {
	out("You have no skills.\n");
	return;
    }

    out("Your skills are:\n");
    foreach ( name, skill in skills )
    {
	outf("%20s: %5d  %5d\n",
	     name, skill->skill_points, skill->training_points);
    }

#endif /* USE_SKILLS */
}
