/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** This provides basic skill support for npcs
** More extensive skill support for players is found in /std/body/skills.c
*/

#include <skills.h>

int query_level();

//:FUNCTION aggregate_skill
// This provides a very simple skill function for basic npcs -
// value of all skills is just the level
// It is overloaded by a more complex system for players
int aggregate_skill(string skill)
{
  return query_level();
}

//:FUNCTION base_test_skill
// Test whether this adversary' skill succeeds against a given opposing skill
// level.  Returns 1 for success of this user.
int base_test_skill(string skill, int opposing_skill)
{
  int total_skill;
  int amount;
  int combined_total;

  total_skill = aggregate_skill(skill);
  combined_total = total_skill + opposing_skill;

/*
 ** If both participants are unskilled, then make them evenly matched.
 ** (and avoid divide by zero errors :-)
 */
  if ( !combined_total )
  {
    total_skill = opposing_skill = 1;
    combined_total = 2;
  }

/*
 ** total_skill should be total_skill/opposing_skill more likely to win
 ** than opposing_skill.  The formula below works this way.  For example,
 ** total_skill == 100, opposing_skill == 50.  This user has a 2/3 chance
 ** of winning.
 */
  if ( random(combined_total) < opposing_skill ) // FAILED
    return 0;

  return 1;
}

//:FUNCTION test_skill
// This is the basic skill test for adversaries.
// For players it is replaced by an extended version, supporting the
// improvement of skills through use.
int test_skill(string skill, int opposing_skill)
{
  return base_test_skill(skill, opposing_skill);
}
