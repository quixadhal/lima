/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** DESIGN
**
**   Skills are represented as slash-separated names, organized as a tree
**   of skills.  Learning a skill propagates improvement up the chain to
**   the parent skills.  The total, aggregate value of a skill at any point
**   is computed from a skill and all of its parent skills.  This implies
**   that use of one skill can benefit sibling skills; for example, using a
**   sword improves the general combat skill which can help when you are
**   forced to use a polearm (which you are otherwise unskilled at).
**
**   A secondary concept of "training points" exist for each skill.  These
**   represent potential, additional improvement through formal training.
**   Similar to skill values, the training points also propagate upwards.
**   The points can then be applied to the skill and/or back down the tree;
**   for example, using a sword earns training points in combat which can
**   then be applied towards formal training with polearms or archery.
**
** MECHANICS
**
**   Progression within a skill is based on a curve (specifically, a third-
**   order polynomial).  At the low- and top-end of the skill value range,
**   it is hard to advance.  This is to represent the initial difficulty
**   of learning a new skill and the difficulty of perfecting it.  In the
**   middle of the skill value range, growth is much faster (learned points
**   are applied at a near one-to-one ratio).
**
**   At the ends of the range, skills are gained in a proportion based on
**   the LEARNING_FACTOR symbol.
**
**   As learning a skill propogates up the tree of skills, the amount
**   applied decreases exponentially (set by PROPOGATION_FACTOR).
**
**   Training points are added proportionally to the increase in the main
**   skill (set by the TRAINING_FACTOR symbol).
**
**   Learning a skill is based on three symbols: SKILL_ON_FAILURE,
**   SKILL_MIN_ON_WIN, SKILL_MAX_ON_WIN.  When a test_skill() fails, the
**   SKILL_ON_FAILURE amount is learned.  When the test_skill() succeeds,
**   then you learn something in the range of MIN/MAX based on your ratio
**   against the opposing skill (MAX if you're totally outclassed or MIN
**   if you stomp all over your opponent).
**
**   All these parameters are set in the skills.h file.
**
** Note: policy decision says that we aren't protecting skills from
**       "unauthorized" tampering.  This is consistent with much of
**       lib -- wizards can help players in any numbers of ways and
**       we won't try to guard against all of them.
*/

#include <classes.h>
#include <skills.h>

inherit CLASS_SKILL;

private mapping skills = ([ ]);

int base_test_skill(string skill, int opposing_skill);

class skill set_skill(string skill, int skill_points, int training_points)
{
    class skill	cs = skills[skill];

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
    return skills;
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
	int s;

	my_skill = skills[skill];
	if ( !my_skill )
	{
	    /* use set_skill() for verification of the skill */
	    my_skill = set_skill(skill, 0, 0);
	}

	/* centered within skill range */
	s = my_skill->skill_points - (MAX_SKILL_VALUE / 2);

	/* as a person's skill increases, the amount they learn decreases */
	divisor = ((LEARNING_FACTOR - 1) * s * s) / (MAX_SKILL_VALUE * MAX_SKILL_VALUE / 4) + 1;
	my_skill->skill_points += fuzzy_divide(value, divisor);
	if ( my_skill->skill_points > MAX_SKILL_VALUE )
	    my_skill->skill_points = MAX_SKILL_VALUE;

	/* accum training points */
	my_skill->training_points += fuzzy_divide(value, divisor * TRAINING_FACTOR);

	/* as skill moves up tree, it decreases */
	value = fuzzy_divide(value, PROPAGATION_FACTOR);
	if ( !value )
	    break;

	i = strsrch(skill, '/', -1);
	if ( i == -1 )
	    break;
	skill = skill[0..i-1];
    }
}

//:FUNCTION test_skill
// This replaces the basic adversary test_skill function,
// adding an attempt to improve the skill
int test_skill(string skill, int opposing_skill)
{
  int total_skill;
  int amount;
  int combined_total;
  int res;

  total_skill = aggregate_skill(skill);
  combined_total = total_skill + opposing_skill;

  res = base_test_skill(skill, opposing_skill);

  if(res)
  {
/*
 ** Successful skill attempts win a number of skill points based on
 ** the ratio between the total_skill and opposing_skill.
 **
 ** The range is MIN to MAX, centered between the two for evenly matched
 ** skills.  As the opposing skill increases, so does the amount learned.
 */
    amount = ( ( (SKILL_MAX_ON_WIN - SKILL_MIN_ON_WIN) * opposing_skill
           + (combined_total / 2) )
           / combined_total
           + SKILL_MIN_ON_WIN
           );
    learn_skill(skill, amount);
  }
  else
    learn_skill(skill, SKILL_ON_FAILURE);
  return res;
}


//:FUNCTION query_evaluation
//Returns the player's overall evaluation (0 to 100 percent) of their skill
//level.  This evaluation corresponds to how they are doing with respect
//to the maximum possible skill level.
int query_evaluation()
{
    return implode(values(skills),
		   (: $1 + ((class skill)$2)->skill_points :),
		   0) * 100 / EVALUATION_SKILL_LEVEL;
}
