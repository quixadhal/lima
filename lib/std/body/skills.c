/* Do not remove the headers from this file! see /USAGE for more info. */

#include <classes.h>
#include <skills.h>

inherit CLASS_SKILL;

private mapping skills = ([]);


void set_skill(string skill, int skillpoints, int level)
{
  string* 	pieces;
  string 	piece;
  class skill	current_skill;
  
  pieces = explode(skill,"/");
  current_skill = skills[pieces[0]];
  if(!current_skill)
    {
      skills[pieces[0]] = new(class skill);
      ((class skill)skills[pieces[0]])->subskills = ([]);
      current_skill = skills[pieces[0]];
    }
  foreach(piece in pieces[1..])
    {
      if(!(current_skill->subskills[piece]))
	{
	  current_skill->subskills = ([piece: new(class skill)]);
	  ((class skill)current_skill->subskills[piece])->subskills = ([]);
	}
      current_skill = current_skill->subskills[piece];
    }
    current_skill->skill_points = skillpoints;
    current_skill->level = level;
}


mapping get_skills()
{
  return copy(skills);
}


class skill get_skill(string skill, mixed value)
{
  string* 	pieces;
  string 	piece;
  class skill	current_skill;
  int*		higher_levels;
  int           i;
  
  pieces = explode(skill,"/");
  current_skill = skills[pieces[0]];
  if(!current_skill)
    {
      return 0;
    }
  higher_levels = allocate(sizeof(pieces)-1);
  for(i=1; i < sizeof(pieces); i++)
    {
      higher_levels[i-1] = current_skill->level;
      piece = pieces[i];
      if(!(current_skill->subskills[piece]))
	{
	  return 0;
	}
      current_skill = current_skill->subskills[piece];
    }
    current_skill->higher_levels = higher_levels;
    return current_skill;
}
