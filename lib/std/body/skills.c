/* Do not remove the headers from this file! see /USAGE for more info. */

#include <classes.h>

inherit CLASS_SKILL;

private mapping skills = ([]);

#define LEARNING_FACTOR		10	/* divide new by 1/N of current */
#define TRAINING_FACTOR		10	/* 1/N goes to training pts */
#define PROPAGATION_FACTOR	2	/* 1/N propagates upward */
#define AGGREGATION_FACTOR	3	/* 1/N^i of parent skills aggregate
					   into the specified skill */


class skill set_skill(string skill, int skill_points, int training_points)
{
    class skill	cs = skills[skill];

//### does this func need protections?

    if ( member_array(skill, SKILL_D->query_skills()) == -1 )
	error("illegal skill; cannot set new skill values.\n");

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

//:FUNCTION aggregate_skill
// Returns the aggregated skill value for the specified skill.  This value
// incorporates the value of the skill itself plus all values of the parent
// skills.
int aggregate_skill(string skill)
{
    int total_skill = 0;
    int coef = 1;

    while ( 1 )
    {
	class skill my_skill;
	int i;

	my_skill = skills[skill];
	if ( my_skill )
	{
	    total_skill += fuzzy_divide(my_skill->skill_points, coef);
	}

	coef = coef * AGGREGATION_FACTOR;

	i = strsrch(skill, '/', -1);
	if ( i == -1 )
	    break;
	skill = skill[0..i-1];
    }

    return total_skill;
}

//:FUNCTION learn_skill
// Add some number of skill points to the given skill, propogating skill
// values up through the parent skills.  Training points are also assigned
// as appropriate.
void learn_skill(string skill, int value)
{
    while ( 1 )
    {
	class skill my_skill;
	int divisor;
	int i;

	my_skill = skills[skill];
	if ( !my_skill )
	{
	    /* use set_skill() for verification of the skill */
	    my_skill = set_skill(skill, 0, 0);
	}

	/* as a person's skill increases, the amount they learn decreases */
	divisor = my_skill->skill_points / LEARNING_FACTOR;
	if ( divisor == 0 )
	    divisor = 1;
	my_skill->skill_points += fuzzy_divide(value, divisor);

	/* accum training points */
	my_skill->training_points += fuzzy_divide(value, TRAINING_FACTOR);

	/* as skill moves up tree, it decreases */
	value = fuzzy_divide(value, PROPAGATION_FACTOR);

	i = strsrch(skill, '/', -1);
	if ( i == -1 )
	    break;
	skill = skill[0..i-1];
    }
}

//:FUNCTION test_skill
// Test whether this users' skill succeeds against a given opposing skill
// level.  Returns 1 for success of this user.
int test_skill(string skill, int opposing_skill)
{
    int total_skill;

    total_skill = aggregate_skill(skill);

//### Shouldn't testing a skill automatically train it?

    // total_skill should be total_skill/opposing_skill more likely to win
    // than opposing_skill.  The formula below works this way.  For example,
    // total_skill == 100, opposing_skill == 50.  This user has a 2/3 chance
    // of winning.

    return random(total_skill + opposing_skill) >= opposing_skill;
}
