/* Do not remove the headers from this file! see /USAGE for more info. */

#include <security.h>
#include <classes.h>

#define SKILL_SAVE_FILE "/data/daemons/skill_d"

inherit M_ACCESS;
inherit CLASS_SKILL;

private mapping skills = ([]);

#define TRAINING_FACTOR		10	/* 10% goes to training pts */
#define PROPAGATION_FACTOR	2	/* half propagates upward */
#define AGGREGATION_FACTOR	3	/* 3^N of parent skills aggregate into
					   the specified skill */


void save_me()
{
    unguarded(1, (: save_object, SKILL_SAVE_FILE :));
}

private void create()
{
    restore_object(SKILL_SAVE_FILE);
}


int register_skill(string skill, string handler)
{
    if ( skills[skill] )
    {
	return 0;
    }

    skills[skill]= handler;
    save_me();
    return 1;
}

int remove_skill(string skill)
{
    if ( !skills[skill] )
    {
	return 0;
    }
    if ( base_name(previous_object()) != skills[skill] && !check_privilege(1) )
    {
	return 0;
    }
    map_delete(skills, skill);
    return 1;
}

int aggregate_skill(string skill, object skill_user)
{
    int total_skill = 0;
    int coef = 1;

    while ( 1 )
    {
	class skill my_skill;
	int i;

	my_skill = skill_user->get_skill(skill);
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

varargs void generic_train_skill(string skill, object trainee, int value)
{
    while ( 1 )
    {
	class skill my_skill;
	int i;

	my_skill = trainee->get_skill(skill);
	if ( !my_skill )
	{
	    my_skill = trainee->set_skill(skill, 0, 0);
	}

	my_skill->skill_points += value;
	my_skill->training_points += value * TRAINING_FACTOR;

	value = fuzzy_divide(value, PROPAGATION_FACTOR);

	i = strsrch(skill, '/', -1);
	if ( i == -1 )
	    break;
	skill = skill[0..i-1];
    }
}


varargs int generic_test_skill(string skill,
			       object skill_user,
			       int opposing_skill)
{
    int total_skill;


    total_skill = aggregate_skill(skill, skill_user);

// Shouldn't testing a skill automatically train it?

//### we now have total_skill vs. opposing_skill... what to do?
//total_skill should be total_skill/opposing_skill more likely to win than
//opposing_skill.  So we should pick rand(total_skill+opposing_skill),
//and if it is >= opposing skill, we win.

    return random(total_skill+opposing_skill) >= opposing_skill;
}


// We can train and test a skill that hasn't been registered.
// You only have to register special skills.
varargs void train_skill(string skill, object trainee, mixed rest ...)
{
    string base_skill = explode(skill, "/")[0];

    if ( !skills[base_skill] )
    {
	generic_train_skill(skill, trainee, rest ...);
	return;
    }
    skills[base_skill]->train_skill(skill, trainee, rest ...);
}

varargs int test_skill(string skill,object skill_user, mixed rest ...)
{
    string base_skill = explode(skill,"/")[0];

    if ( !skills[base_skill] )
    {
	return generic_test_skill(skill, skill_user, rest ...);
    }
    return skills[base_skill]->test_skill(skill, skill_user, rest ...);
}
