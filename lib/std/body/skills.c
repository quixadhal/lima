/* Do not remove the headers from this file! see /USAGE for more info. */

#include <classes.h>

inherit CLASS_SKILL;

private mapping skills = ([]);


class skill set_skill(string skill, int skill_points, int training_points)
{
    class skill	cs = skills[skill];

//### does this func need protections?

    if ( !cs )
    {
	cs = skills[skill] = new(class skill,
				 skill_points : skill_points,
				 training_points : training_points);
    }
    else
    {
	cs->skill_points = skill_points;
	cs->training_points = training_points;
    }

    return cs;
}


mapping get_skills()
{
    return copy(skills);
}


class skill get_skill(string skill)
{
    return skills[skill];
}
