/* Do not remove the headers from this file! see /USAGE for more info. */

#include <security.h>
#include <classes.h>
#include <skills.h>
#define SKILL_SAVE_FILE "/data/daemons/skill"

// Beek added some comments, search for *Beek*
// I like the structure, algorithm, etc.  Just some comments on the imp.

inherit M_ACCESS;
inherit CLASS_SKILL;

private mapping skills = ([]);


void save_me(){
  unguarded(1,(:save_object, SKILL_SAVE_FILE:));
}

private void create()
{
  restore_object(SKILL_SAVE_FILE);
}


int register_skill(string skill, string handler){
  if(skills[skill])
    {
      return 0;
    }

  skills[skill] = handler;
  save_me();
  return 1;
}

int remove_skill(string skill)
{
  if(!skills[skill])
    {
      return 0;
    }
  if(base_name(previous_object()) != skills[skill] && !check_privilege(1))
    {
      return 0;
    }
  map_delete(skills, skill);
  return 1;
}

private int calculate_level(int skillpoints, int level)
{
  return skillpoints > points_to_level[level-1] ?
    level + 1 : level;
}

varargs void generic_train_skill(string skill, object trainee, int value)
{
  class skill my_skill;
  string* pieces;
  int i, p;

  my_skill = trainee->get_skill(skill);
  if(!my_skill)
    {
      trainee->set_skill(skill,0);
    }
  pieces = explode(skill,"/");
  my_skill = trainee->get_skill(pieces[0]);
  p = pow(2,sizeof(pieces)-1);
  for(i=1;i<sizeof(pieces);i++)
    {
      my_skill->skill_points += fuzzy_divide(value, p);
      my_skill->level = 
	calculate_level(my_skill->skill_points, my_skill->level);
      p/=2;
      my_skill = (my_skill->subskills)[pieces[i]];
    }
}


varargs int generic_test_skill(string skill, object skill_user, float opposing_skill)
{
  class skill my_skill;
  int coef, i;
  float total_skill;
 
  my_skill = skill_user->get_skill(skill);
  if(!my_skill)
    return 0;

  total_skill = my_skill->level - opposing_skill;
  coef = pow(2,sizeof(my_skill->higher_levels));
  for(i=0;i<sizeof(my_skill->higher_levels);i++)
    {
      total_skill += fuzzy_divide(my_skill->higher_levels,coef);
      coef /= 2;
    }
//### This is obviously wrong, but it isn't obvious what is right
#if 0
    return random(10) - 5 >= 0;
#else
//###I think this is right, but should be checked. -Beek
    return random(10) - 5 >= total_skill;
#endif
}


// We can train and test a skill that hasn't been registered.
// You only have to register special skills.
varargs
int train_skill(string skill, object trainee, mixed rest ...){
  string base_skill = explode(skill, "/")[0];

  if(!skills[base_skill])
    {
      generic_train_skill(skill, trainee, rest ...);
      return;
    }
  skills[base_skill]->train_skill(skill, trainee, rest ...);
}

varargs
int test_skill(string skill,object skill_user, mixed rest ...){
  string 	base_skill = explode(skill,"/")[0];

  if(!skills[base_skill])
    {
      generic_test_skill(skill, skill_user, rest ...);
    }
  skills[base_skill]->test_skill(skill, skill_user, rest ...);
}


