/* Do not remove the headers from this file! see /USAGE for more info. */

// Skill points are something the player never sees, but he gets or
// loses every time he uses a skill.  Level is the level the player
// sees.  Subskills If attack is a skill, attack/sword is a subskill
// Special skill indicates that the skill defines it's own levels
// Higher levels is a dynamic variable that's changed each time you
// get the skill from a user (if needed) which tells you the value of
// the parent skills.

class skill{
  int 		skill_points;
  int 		level;
  mapping 	subskills;
  int 		special_skill;
  int*		higher_levels;
}


