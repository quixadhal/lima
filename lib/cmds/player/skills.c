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

    mapping skills = this_body()->get_skills();
    string array names;

    if ( sizeof(skills) == 0 )
    {
	out("You have no skills.\n");
	return;
    }
    names = sort_array(keys(skills), 1);
    outf("%-20s %8s  %8s\n%*'-'s\n", "Skill", "Points", "Training", 50, "");

    foreach ( string name in names )
    {
	class skill skill = skills[name];
	string * parts = explode(name, "/");
	string name2 = repeat_string("   ", sizeof(parts)-1) + parts[<1];
	
	outf("%-20s %8d  %8d\n",
	     name2, skill->skill_points, skill->training_points);
    }

#endif /* USE_SKILLS */
}
